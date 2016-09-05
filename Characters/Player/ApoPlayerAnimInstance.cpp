// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerAnimInstance.h"

#include "Characters/Player/ApoPlayerCharacter.h"
#include "Characters/Player/ApoPlayerMovementComponent.h"

void UApoPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AApoPlayerCharacter>(TryGetPawnOwner());
	if (Owner)
	{
		MovementComponent = Cast<UApoPlayerMovementComponent>(Owner->GetMovementComponent());
	}
}

bool UApoPlayerAnimInstance::IsShooting() const
{
	if (!Owner)
	{
		return false;
	}

	return Owner->IsShooting();
}

bool UApoPlayerAnimInstance::IsAiming() const
{
	if (!Owner)
	{
		return false;
	}

	return Owner->IsAiming();
}

bool UApoPlayerAnimInstance::IsJogging() const
{
	if (!MovementComponent)
	{
		return false;
	}

	return MovementComponent->IsWalking();
}

bool UApoPlayerAnimInstance::IsJumping() const
{
	if (!MovementComponent)
	{
		return false;
	}

	return MovementComponent->IsFalling();
}

bool UApoPlayerAnimInstance::IsCrouching() const
{
	if (!MovementComponent)
	{
		return false;
	}

	return MovementComponent->IsCrouching();
}

float UApoPlayerAnimInstance::GetCurrentMovingSpeed() const
{
	if (!MovementComponent)
	{
		return 0.f;
	}

	return MovementComponent->GetCurrentMovingSpeed();
}

float UApoPlayerAnimInstance::GetCurrentMovingAngle() const
{
	if (!MovementComponent)
	{
		return 0.f;
	}

	return MovementComponent->GetCurrentMovingAngle();
}

float UApoPlayerAnimInstance::GetCurrentPitch() const
{
	if (!Owner)
	{
		return 0.0f;
	}

	float Pitch = Owner->GetControlRotation().Pitch;
	if (Pitch > 180.f)
	{
		Pitch -= 360.f;
	}

	return Pitch;
}



