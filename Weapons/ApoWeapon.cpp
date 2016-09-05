// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoWeapon.h"

#include "Effects/ApoImpactEffectSpawner.h"
#include "Characters/Player/ApoPlayerCharacter.h"
#include "Characters/Player/ApoPlayerStimuliSourceComponent.h"
#include "UI/ApoGameHUD.h"

// Sets default values
AApoWeapon::AApoWeapon() : NumBulletsMax(150), NumBullets(150), NumBulletsPerMagazine(30), 
NumBulletsInMagazine(0), MaxShotDistance(5000.f), DelayBetweenShots(0.02f), ShotDamage(10),
LastShotTime(0.f), bWantsToShoot(false), WeaponState(EApoWeaponState::Unequipped)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShootingAudioComponent = CreateDefaultSubobject<UAudioComponent>("ShootingAudio");
	ShootingAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShootingAudioComponent->bAutoActivate = false;

	ShootingParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ShootingParticles");
	ShootingParticleSystemComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, "MuzzleFlashSocket");
	ShootingParticleSystemComponent->bAutoActivate = false;
}

void AApoWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsShooting())
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastShotTime > DelayBetweenShots)
		{
			LastShotTime = CurrentTime;
			
			Shoot();
		}
	}
}

void AApoWeapon::StartShooting()
{
	bWantsToShoot = true;

	// We can start shoot only if we are not reloading or we aren't already shooting
	if (WeaponState == EApoWeaponState::Idle)
	{
		// If magazine is empty then we should reload before shooting
		if (IsEmpty())
		{
			Reload();
		}
		else
		{
			EnableShooting();
		}
	}
}

void AApoWeapon::StopShooting()
{
	bWantsToShoot = false;
	DisableShooting();
}

void AApoWeapon::Reload()
{
	// If process of reloading already begun or it is finished
	if (IsReloading() || !IsEquipped() || NumBulletsInMagazine == NumBulletsPerMagazine)
	{
		return;
	}

	// Block shooting and turn off any sounds it produces
	if (IsShooting())
	{
		DisableShooting();
	}

	// If we can reload and have enough bullets
	if (NumBullets > 0)
	{
		auto Player = Cast<AApoPlayerCharacter>(GetOwner());
		if (Player && ReloadAnimMontage)
		{
			WeaponState = EApoWeaponState::Reloading;

			Player->PlayAnimMontage(ReloadAnimMontage);
			GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AApoWeapon::FinishReload, ReloadAnimMontage->GetPlayLength(), false);
		}
	}
	// No bullets left for reloading
	else
	{
		UGameplayStatics::PlaySound2D(this, NoBulletsSoundCue);
	}
}

void AApoWeapon::InterruptReload()
{
	if (!IsReloading())
	{
		return;
	}

	auto Player = Cast<AApoPlayerCharacter>(GetOwner());
	if (Player)
	{
		Player->StopAnimMontage(ReloadAnimMontage);
	}
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	// Reload was unsuccessfull, but some bullets are in magazine -> enable shooting
	if (bWantsToShoot && NumBulletsInMagazine > 0)
	{
		EnableShooting();
	}
}

void AApoWeapon::RestoreAmmoFull()
{
	// Restore bullets in backpack and in the magazine too, reloading will be automatically called
	NumBullets = NumBulletsMax;
	NumBullets += (NumBulletsPerMagazine - NumBulletsInMagazine);

	Reload();

	// Inform player about new ammo stats, so ammo restoring is visible instantly
	InformOwner();
}

void AApoWeapon::RestoreAmmoInMagazines(int32 NumMagazines)
{
	int32 DesiredNumBullets = NumBullets + NumMagazines * NumBulletsPerMagazine;
	int32 MaxNumBullets = NumBulletsMax + NumBulletsPerMagazine - NumBulletsInMagazine;
	NumBullets = FMath::Min(DesiredNumBullets, MaxNumBullets);

	// Inform player about new ammo stats, so ammo restoring is visible instantly
	InformOwner();
}

void AApoWeapon::SetAmmoInBullets(int32 BulletsInBackpack, int32 BulletsInMagazine, bool bReload)
{
	NumBullets = FMath::Min(BulletsInBackpack, NumBulletsMax);
	NumBulletsInMagazine = FMath::Min(BulletsInMagazine, NumBulletsPerMagazine);

	if (bReload && (NumBullets > 0 || NumBulletsInMagazine > 0))
	{
		NumBullets += NumBulletsInMagazine;
		NumBulletsInMagazine = 0;
		Reload();
	}
	else
	{
		InformOwner();
	}
}

void AApoWeapon::SetAmmoInMagazines(int32 NumMagazines, bool bReload)
{
	if (NumMagazines < 1)
	{
		SetAmmoInBullets(0, 0, false);
	}
	else
	{
		NumBullets = (NumMagazines - 1) * NumBulletsPerMagazine;
		NumBulletsInMagazine = NumBulletsPerMagazine;

		SetAmmoInBullets(NumBullets, NumBulletsInMagazine, bReload);
	}
}

void AApoWeapon::EnableShooting()
{
	if (WeaponState != EApoWeaponState::Shooting)
	{
		WeaponState = EApoWeaponState::Shooting;
		ShootingAudioComponent->Play();
		ShootingParticleSystemComponent->Activate();
	}
}

void AApoWeapon::DisableShooting()
{
	if (WeaponState == EApoWeaponState::Shooting)
	{
		WeaponState = EApoWeaponState::Idle;
		ShootingAudioComponent->Stop();
		ShootingParticleSystemComponent->Deactivate();
	}
}

void AApoWeapon::Shoot()
{
	// Do not shoot if there is not any ammo in current magazine
	if (IsEmpty())
	{
		Reload();
		return;
	}

	auto Owner = Cast<AApoPlayerCharacter>(GetOwner());
	auto PlayerController = Cast<APlayerController>(Owner->GetController());

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	PlayerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	// Shot bullet through crosshair
	FVector AimDir = ViewPointRotation.Vector();

	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	MeshComponent->GetSocketWorldLocationAndRotation("MuzzleFlashSocket", MuzzleLocation, MuzzleRotation);

	// Move start location from camera to muzzle
	FVector Start = ViewPointLocation + AimDir *(FVector::DotProduct(MuzzleLocation - ViewPointLocation, AimDir));

	FHitResult HitResult = TraceShot(Start, AimDir);
	if (HitResult.bBlockingHit)
	{
		SpawnImpactEffect(HitResult);

		FPointDamageEvent PointDamageEvent(ShotDamage, HitResult, AimDir, nullptr);
		HitResult.GetActor()->TakeDamage(ShotDamage, PointDamageEvent, Instigator->GetController(), Instigator);
	}

	Owner->GetStimuliSourceComponent()->MakeNoise(GetActorLocation(), 2000.f, 1.f, "Shooting");
	
	// Quick hack for auto reloading, when magazine is empty
	if (--NumBulletsInMagazine == 0)
	{
		Reload();
	}

	// Inform player about new weapon states
	InformOwner();
}

void AApoWeapon::SpawnImpactEffect(FHitResult HitResult)
{
	TActorIterator<AApoImpactEffectSpawner> EffectSpawnerIt(GetWorld());
	if (EffectSpawnerIt)
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
		FVector SpawnLocation = HitResult.ImpactPoint;
		FRotator SpawnRotation = HitResult.ImpactPoint.Rotation();

		EffectSpawnerIt->SpawnImpactEffect(SurfaceType, SpawnLocation, SpawnRotation);
	}
}

void AApoWeapon::FinishReload()
{
	// Merge bullets which are left in magazine with others
	NumBullets += NumBulletsInMagazine;

	// Fill up magazine with bullets(either full or partial) 
	NumBulletsInMagazine = NumBullets >= NumBulletsPerMagazine ? NumBulletsPerMagazine : NumBullets;
	NumBullets -= NumBulletsInMagazine;

	// If shooting was disabled by reloading and player wants to shoot, enable it
	if (bWantsToShoot)
	{
		EnableShooting();
	}
	else
	{
		WeaponState = EApoWeaponState::Idle;
	}

	// Inform player about new weapon states
	InformOwner();
}

FHitResult AApoWeapon::TraceShot(FVector Start, FVector AimDir)
{
	FCollisionQueryParams QueryParams("WeaponTrace", true, Instigator);
	QueryParams.bReturnPhysicalMaterial = true;

	FVector End = Start + AimDir * MaxShotDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_BULLET, QueryParams);

	return HitResult;
}

void AApoWeapon::InformOwner()
{
	auto Player = Cast<AApoPlayerCharacter>(GetOwner());
	if (Player && IsEquipped())
	{
		Player->GetGameHUD()->UpdateWeaponWidget(NumBullets, NumBulletsInMagazine);
	}
}

