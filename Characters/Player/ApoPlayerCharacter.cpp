// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerCharacter.h"

#include "Characters/Player/ApoInventoryComponent.h"
#include "Characters/Player/ApoPlayerController.h"
#include "Characters/Player/ApoPlayerMovementComponent.h"
#include "Characters/Player/ApoPlayerStimuliSourceComponent.h"
#include "Items/ApoUsableActor.h"
#include "UI/ApoGameHUD.h"
#include "Weapons/ApoWeapon.h"

AApoPlayerCharacter::AApoPlayerCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UApoPlayerMovementComponent>(AApoPlayerCharacter::CharacterMovementComponentName)),
	UsableActorMaxDistance(200.f), bAiming(false), UsableActorInView(nullptr)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	PrimaryActorTick.bCanEverTick = true;

	auto MovementComponent = static_cast<UApoPlayerMovementComponent*>(GetMovementComponent());
	MovementComponent->NavAgentProps.bCanCrouch = true;
	MovementComponent->CrouchedHalfHeight = 60.f;
	MovementComponent->JumpZVelocity = 325.f;
	MovementComponent->MaxWalkSpeed = 400.f;
	MovementComponent->MaxWalkSpeedCrouched = 175.f;

	ThridPersonArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	ThridPersonArmComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ThridPersonArmComponent->TargetArmLength = 300.f;
	ThridPersonArmComponent->TargetOffset = FVector(0.f, 0.f, 55.f);
	ThridPersonArmComponent->SocketOffset = FVector(0.f, 35.f, 0.f);
	ThridPersonArmComponent->bUsePawnControlRotation = true;
	ThridPersonArmComponent->bEnableCameraLag = true;

	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera");
	ThirdPersonCameraComponent->AttachToComponent(ThridPersonArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ThirdPersonCameraComponent->SetRelativeLocation(FVector(135.f, 25.f, 20.f));
	ThirdPersonCameraComponent->bAutoActivate = true;

	DeathArmComponent = CreateDefaultSubobject<USpringArmComponent>("DeathArm");
	DeathArmComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DeathArmComponent->TargetArmLength = 300.f;
	DeathArmComponent->TargetOffset = FVector(0.f, 0.f, 55.f);
	DeathArmComponent->SocketOffset = FVector(0.f, 35.f, 0.f);
	DeathArmComponent->bAutoActivate = false;

	DeathCameraComponent = CreateDefaultSubobject<UCameraComponent>("DeathCamera");
	DeathCameraComponent->AttachToComponent(DeathArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathCameraComponent->bAutoActivate = false;

	InventoryComponent = CreateDefaultSubobject<UApoInventoryComponent>("Inventory");

	StimuliSourceComponent = CreateDefaultSubobject<UApoPlayerStimuliSourceComponent>("PlayerStimuliSource");
}

void AApoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto GameHUD = GetGameHUD();
	if (GameHUD)
	{
		GameHUD->ShowCrosshair();
		GameHUD->UpdateHealthWidget(HealthPoints, HealthPointsMax);
	}

	StimuliSourceComponent->RegisterPawnAsSource(this);
}

void AApoPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Find usable actor on who we are looking at
	auto UsableActor = GetUsableActorInView();
	if (UsableActor)
	{
		// If we are looking at something new
		if (UsableActor != UsableActorInView)
		{
			// Inform old one that it is not focused usable actor
			if (UsableActorInView)
			{
				UsableActorInView->OnLostFocus(this);
			}
			
			// Focus on new one
			UsableActorInView = UsableActor;
			UsableActorInView->OnFocus(this);
		}
	}
	// If we lost focus on old one
	else if (UsableActorInView)
	{
		// Inform old one that it is not focused usable actor
		UsableActorInView->OnLostFocus(this);
		UsableActorInView = nullptr;
	}

	auto MovementComponent = Cast<UApoPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->GetCurrentMovingSpeed() > 0.f)
	{
		// If we are aiming, we are moving silently(slowly) 
		// If we are crouched, we are moving silently(slowly)
		if (!IsAiming() && !bIsCrouched)
		{
			GetStimuliSourceComponent()->MakeNoise(GetActorLocation(), 1250.f, 0.5f, "LoudMoving");
		}
	}
}

void AApoPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StimuliSourceComponent->UnregisterPawnAsSource();
}

void AApoPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AApoPlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AApoPlayerCharacter::MoveRight);

	InputComponent->BindAxis("LookUp", this, &AApoPlayerCharacter::LookUp);
	InputComponent->BindAxis("Turn", this, &AApoPlayerCharacter::Turn);

	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnCrouch);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AApoPlayerCharacter::OnUnCrouch);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::Jump);

	InputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnStartShooting);
	InputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &AApoPlayerCharacter::OnStopShooting);

	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnStartAiming);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AApoPlayerCharacter::OnStopAiming);

	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnReload);

	InputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnUseUsable);
	
	InputComponent->BindAction("ShowObjective", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::OnShowCurrentObjective);

	InputComponent->BindAction("Quit", EInputEvent::IE_Pressed, this, &AApoPlayerCharacter::Quit);
}

float AApoPlayerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	GetGameHUD()->UpdateHealthWidget(HealthPoints, HealthPointsMax);
	
	return static_cast<float>(HealthPoints);
}

void AApoPlayerCharacter::Die(FDamageEvent const& DamageEvent)
{
	if (IsDying())
	{
		return;
	}

	// Player can not aim when he is dying
	if (IsAiming())
	{
		OnStopAiming();
	}

	// Player can not jump when he is dying
	if (IsJumpProvidingForce())
	{
		StopJumping();
	}

	// Player can not crouch when he is dying
	if (bIsCrouched)
	{
		OnUnCrouch();
	}

	// Player can not use weapon when he is dying
	auto EquippedWeapon = InventoryComponent->GetEquippedWeapon();
	if (EquippedWeapon)
	{
		if (EquippedWeapon->IsShooting())
		{
			EquippedWeapon->StopShooting();
		}
		if (EquippedWeapon->IsReloading())
		{
			EquippedWeapon->InterruptReload();
		}
	}
	
	// Switch to death camera
	ThirdPersonCameraComponent->Deactivate();
	ThridPersonArmComponent->Deactivate();

	DeathArmComponent->Activate();
	DeathCameraComponent->Activate();

	// Update HUD to death state
	auto GameHUD = Cast<AApoGameHUD>(Cast<APlayerController>(GetController())->GetHUD());
	if (GameHUD)
	{
		GameHUD->HideCrosshair();
	}

	Super::Die(DamageEvent);
}

bool AApoPlayerCharacter::IsShooting() const
{
	auto EquippedWeapon = InventoryComponent->GetEquippedWeapon();

	return EquippedWeapon ? EquippedWeapon->IsShooting() : false;
}

void AApoPlayerCharacter::RestoreHealthPoints(float RestoreCoef)
{
	HealthPoints = FMath::Min(HealthPoints + static_cast<int>(RestoreCoef * HealthPointsMax), HealthPointsMax);

	GetGameHUD()->UpdateHealthWidget(HealthPoints, HealthPointsMax);
}

void AApoPlayerCharacter::UpdateGameplayObjective(const FString &Objective, const FString &Desc)
{
	auto GameHUD = GetGameHUD();
	if (GameHUD)
	{
		GameHUD->SetGameplayObjective(Objective, Desc);
		GameHUD->ShowGameplaySummary();
	}
}

AApoWeapon * AApoPlayerCharacter::GetEquippedWeapon() const
{
	return InventoryComponent->GetEquippedWeapon();
}

AApoGameHUD* AApoPlayerCharacter::GetGameHUD()
{
	auto PlayerController = Cast<AApoPlayerController>(GetController());
	return PlayerController ? Cast<AApoGameHUD>(PlayerController->GetHUD()) : nullptr;
}

AApoUsableActor * AApoPlayerCharacter::GetUsableActorInView()
{
	auto PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return nullptr;
	}

	// Take view direction from crosshair
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	PlayerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	FVector ViewDir = ViewPointRotation.Vector();
	
	// Move start location from camera to player
	FVector Start = ViewPointLocation + ViewDir * (FVector::DotProduct(GetActorLocation() - ViewPointLocation, ViewDir));
	
	// Make little offset from start distance, because when usable actor is too big(e.g ammo box)
	// we can be pointing on it and it will be in distance, but we will make little move and he won't
	// be in distance, therefore we will be controlling distance from center, not distance from impact point
	FVector End = Start + ViewDir * (UsableActorMaxDistance * 2.f);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceAsyncScene = true;
	QueryParams.bTraceComplex = false;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_USABLE, QueryParams);
	
	if (HitResult.bBlockingHit)
	{
		// Check distance from actors centers
		if (FVector::Dist(HitResult.GetActor()->GetActorLocation(), GetActorLocation()) > UsableActorMaxDistance && HitResult.Distance > UsableActorMaxDistance)
		{
			return nullptr;
		}
	}

	return Cast<AApoUsableActor>(HitResult.GetActor());
}

void AApoPlayerCharacter::MoveForward(float Axis)
{
	// Skip senseless updates
	if (Axis == 0 || IsDying())
	{
		return;
	}

	GetMovementComponent()->AddInputVector(GetActorForwardVector() * Axis);
}

void AApoPlayerCharacter::MoveRight(float Axis)
{
	// Skip senseless updates
	if (Axis == 0 || IsDying())
	{
		return;
	}
	
	GetMovementComponent()->AddInputVector(GetActorRightVector() * Axis);
}

void AApoPlayerCharacter::LookUp(float Axis)
{
	// Skip senseless updates
	if (IsDying() || Axis == 0.f)
	{
		return;
	}

	AddControllerPitchInput(Axis);
}

void AApoPlayerCharacter::Turn(float Axis)
{
	// Skip senseless updates
	if (IsDying() || Axis == 0.f)
	{
		return;
	}

	AddControllerYawInput(Axis);
}

void AApoPlayerCharacter::Jump()
{
	// Skip senseless updates
	if (IsDying())
	{
		return;
	}

	auto MovementComponent = Cast<UApoPlayerMovementComponent>(GetMovementComponent());

	// Player can jump only if he is moving forward or he is not moving at all
	bool bMoving = MovementComponent->IsWalking() && MovementComponent->GetCurrentMovingSpeed() > 0.f;
	bool bMovingForward = FMath::Abs(MovementComponent->GetCurrentMovingAngle()) < 45.f && bMoving;

	if (!bMoving || bMovingForward)
	{
		// We can not shoot when we are jumping
		if (IsShooting())
		{
			OnStopShooting();
		}

		Super::Jump();
	}
}

void AApoPlayerCharacter::OnCrouch()
{
	// Skip senseless updates
	if (IsDying())
	{
		return;
	}

	Crouch();
}

void AApoPlayerCharacter::OnUnCrouch()
{
	UnCrouch();
}

void AApoPlayerCharacter::OnStartShooting()
{
	// Skip senseless updates
	if (IsDying())
	{
		return;
	}

	auto MovementComponent = Cast<UApoPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->IsWalking() || MovementComponent->IsCrouching())
	{
		auto EquippedWeapon = InventoryComponent->GetEquippedWeapon();
		if (EquippedWeapon)
		{
			EquippedWeapon->StartShooting();
		}
	}
}

void AApoPlayerCharacter::OnStopShooting()
{
	auto EquippedWeapon = InventoryComponent->GetEquippedWeapon();
	if (EquippedWeapon)
	{
		EquippedWeapon->StopShooting();
	}
}

void AApoPlayerCharacter::OnReload()
{
	// Skip senseless updates
	if (IsDying())
	{
		return;
	}

	auto EquippedWeapon = InventoryComponent->GetEquippedWeapon();
	if (EquippedWeapon)
	{
		EquippedWeapon->Reload();
	}
}

void AApoPlayerCharacter::OnStartAiming()
{
	// Skip senseless updates
	if (IsDying())
	{
		return;
	}

	auto MovementComponent = Cast<UApoPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->IsWalking() || MovementComponent->IsCrouching())
	{
		bAiming = true;
	}
}

void AApoPlayerCharacter::OnStopAiming()
{
	bAiming = false;
}

void AApoPlayerCharacter::OnUseUsable()
{
	if (UsableActorInView)
	{
		UsableActorInView->Use(this);
	}
}

void AApoPlayerCharacter::OnShowCurrentObjective()
{
	auto GameHUD = GetGameHUD();
	if (GameHUD)
	{
		GameHUD->IsGameplaySummaryVisibled() ? GameHUD->HideGameplaySummary() : GameHUD->ShowGameplaySummary();
	}
}

void AApoPlayerCharacter::Quit()
{
	auto PlayerController = Cast<AApoPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("quit");
	}
}
