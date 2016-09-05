// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoZombieAnimInstance.h"

#include "Characters/Zombie/ApoZombieCharacter.h"

void UApoZombieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AApoZombieCharacter>(TryGetPawnOwner());
	if (Owner)
	{
		MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
	}
}

bool UApoZombieAnimInstance::IsWalking() const
{
	if (!MovementComponent)
	{
		return false;
	}

	return MovementComponent->IsWalking();
}

float UApoZombieAnimInstance::GetCurrentMovingSpeed() const
{
	if (!MovementComponent)
	{
		return 0.f;
	}

	return MovementComponent->Velocity.Size2D();
}
