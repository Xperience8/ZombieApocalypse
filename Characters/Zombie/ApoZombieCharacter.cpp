// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoZombieCharacter.h"

#include "Characters/Player/ApoPlayerCharacter.h"
#include "Characters/Zombie/ApoZombieController.h"
#include "Gameplay/ApoGameState.h"

AApoZombieCharacter::AApoZombieCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer), HeadDamageFactor(10000.f), BodyDamageFactor(1.f), LimbDamageFactor(0.5f), 
	DelayBetweenAttacks(1.f), MaxDistanceToAttack(100.f), AttackDamage(20.f), CurrentState(EZombieState::None)
{
	PrimaryActorTick.bCanEverTick = false;

	auto MovementComponent = static_cast<UCharacterMovementComponent*>(GetMovementComponent());
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->NavAgentProps.bCanWalk = true;
	MovementComponent->NavAgentProps.bCanCrouch = false;
	MovementComponent->NavAgentProps.bCanJump = false;
	MovementComponent->NavAgentProps.bCanSwim = false;
	MovementComponent->NavAgentProps.bCanFly = false;
	MovementComponent->MaxWalkSpeed = 200.f;
	MovementComponent->MaxWalkSpeedCrouched = 200.f;

	ZombieNoiseComponent = CreateDefaultSubobject<UAudioComponent>("Noise");
	ZombieNoiseComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ZombieNoiseComponent->bAutoActivate = false;
}

void AApoZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Register yourself with gameplay system
	auto GameState = GetWorld()->GetGameState<AApoGameState>();
	if (GameState)
	{
		GameState->OnZombieSpawned();
	}
}

void AApoZombieCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Unregister yourself from gameplay system
	auto GameState = GetWorld()->GetGameState<AApoGameState>();
	if (GameState)
	{
		GameState->OnZombieDied();
	}
}

float AApoZombieCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		auto PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		
		// Multiply damage with specific factor which depends on what part of the body we have hit
		auto SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PointDamageEvent->HitInfo.PhysMaterial.Get());
		if (SurfaceType == ZOMBIE_HEAD_SURFACE_TYPE)
		{
			Damage *= HeadDamageFactor;
		}
		else if (SurfaceType == ZOMBIE_BODY_SURFACE_TYPE)
		{
			Damage *= BodyDamageFactor;
		}
		else
		{
			Damage *= LimbDamageFactor;
		}
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AApoZombieCharacter::Die(FDamageEvent const& DamageEvent)
{
	if (IsDying())
	{
		return;
	}

	// Stops behavior tree
	DetachFromControllerPendingDestroy();
	ChangeZombieState(EZombieState::None);

	Super::Die(DamageEvent);
}

bool AApoZombieCharacter::CanAttack(AApoPlayerCharacter* PlayerToAttack) const
{
	// Check for basic mistakes
	if (!PlayerToAttack || IsDying())
	{
		return false;
	}

	bool bEnoughTimeHasPassed = GetWorld()->GetTimeSeconds() - LastAttackTime > DelayBetweenAttacks;
	bool bIsPlayerReachable = FVector::DistSquared(GetActorLocation(), PlayerToAttack->GetActorLocation()) < MaxDistanceToAttack * MaxDistanceToAttack;
	bool bIsPlayerInFrontOfZombie = FVector::DotProduct(GetActorForwardVector(), (PlayerToAttack->GetActorLocation() - GetActorLocation()).GetUnsafeNormal()) > 0.5;

	return bEnoughTimeHasPassed && bIsPlayerReachable && bIsPlayerInFrontOfZombie;
}

void AApoZombieCharacter::Attack(AApoPlayerCharacter* PlayerToAttack)
{
	if (!CanAttack(PlayerToAttack))
	{
		return;
	}

	LastAttackTime = GetWorld()->GetTimeSeconds();

	FPointDamageEvent PointDamage(AttackDamage, FHitResult(), (PlayerToAttack->GetActorLocation() - GetActorLocation()).GetUnsafeNormal(), nullptr);
	PlayerToAttack->TakeDamage(AttackDamage, PointDamage, GetController(), this);
}

void AApoZombieCharacter::ChangeZombieState(EZombieState ZombieState)
{
	if (CurrentState == ZombieState)
	{
		return;
	}

	CurrentState = ZombieState;
	if (CurrentState == EZombieState::None)
	{
		ZombieNoiseComponent->Stop();
	}
	else
	{
		switch (CurrentState)
		{
		case EZombieState::Idle:
			ZombieNoiseComponent->SetSound(IdleSoundCue);
			break;

		case EZombieState::Wandering:
			ZombieNoiseComponent->SetSound(WanderingSoundCue);
			break;

		case EZombieState::Hunting:
			ZombieNoiseComponent->SetSound(HuntingSoundCue);
			break;
		}

		ZombieNoiseComponent->Play();
	}
}

