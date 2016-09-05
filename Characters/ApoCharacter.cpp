// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoCharacter.h"

// Sets default values
AApoCharacter::AApoCharacter(const FObjectInitializer& ObjectInitializer) :
	HealthPointsMax(100), HealthPoints(100), TimeAsCorpse(3.f), bDying(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Retrace collision with bullets to mesh(for better collision handling)
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_BULLET, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

float AApoCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// If damage should not be applied or we are already dead
	if (Damage == 0.f || IsDying())
	{
		return 0.f;
	}

	HealthPoints -= Damage;
	if (HealthPoints <= 0)
	{
		Die(DamageEvent);
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundCue, GetActorLocation(), GetActorRotation());
	}

	return static_cast<float>(HealthPoints);
}

void AApoCharacter::Die(FDamageEvent const& DamageEvent)
{
	// Do not die multiple times
	if (IsDying())
	{
		return;
	}

	SetLifeSpan(TimeAsCorpse);
	UGameplayStatics::PlaySoundAtLocation(this, DeathSoundCue, GetActorLocation(), GetActorRotation());

	ActivateRagdollPhysics();
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		auto PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		GetMesh()->AddImpulseAtLocation(PointDamageEvent->ShotDirection * 12000, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.BoneName);
	}

	HealthPoints = 0;
	bDying = true;
}

void AApoCharacter::ActivateRagdollPhysics()
{
	// Disable capsule collision
	UCapsuleComponent *Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Enable rigid body simulation
	USkeletalMeshComponent *Mesh = GetMesh();
	Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
	Mesh->SetAllBodiesSimulatePhysics(true);
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeAllRigidBodies();
}
