// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerCameraManager.h"

#include "Characters/Player/ApoPlayerCharacter.h"

AApoPlayerCameraManager::AApoPlayerCameraManager() : AimFOV(60.f)
{
	ViewPitchMin = -90.f;
	ViewPitchMax = 45.f;
}

void AApoPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	auto Player = PCOwner ? Cast<AApoPlayerCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Player)
	{
		float CurrentFOV = GetFOVAngle();
		float TargetFOV = Player->IsAiming() ? AimFOV : DefaultFOV;

		if (CurrentFOV != TargetFOV)
		{
			SetFOV(FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 20.f));
		}
	}

	Super::UpdateCamera(DeltaTime);
}