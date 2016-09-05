// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerMovementComponent.h"

#include "Characters/Player/ApoPlayerCharacter.h"

void UApoPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = static_cast<AApoPlayerCharacter*>(GetOwner());
}

float UApoPlayerMovementComponent::GetMaxSpeed() const
{
	if (PlayerCharacter->IsAiming())
	{
		return MaxAimSpeed;
	}

	return Super::GetMaxSpeed();
}

float UApoPlayerMovementComponent::GetCurrentMovingAngle() const
{
	FVector ForwardVector = GetOwner()->GetActorForwardVector();

	float DeltaAngle = FMath::FindDeltaAngleRadians(ForwardVector.HeadingAngle(), Velocity.HeadingAngle());

	return FMath::RadiansToDegrees(DeltaAngle);
}


