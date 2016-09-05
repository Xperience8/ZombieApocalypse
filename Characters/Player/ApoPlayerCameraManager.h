// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "ApoPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	/** Restrict pitch rotation*/
	AApoPlayerCameraManager();

	/** Zoom camera if player is aiming*/
	virtual void UpdateCamera(float DeltaTime) override;

private:
	/** FOV which is used when player is aiming*/
	UPROPERTY(EditAnywhere, Category = "PlayerCameraManager")
	float AimFOV;
};
