// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerController.h"

#include "Characters/Player/ApoPlayerCameraManager.h"

AApoPlayerController::AApoPlayerController() 
{
	PlayerCameraManagerClass = AApoPlayerCameraManager::StaticClass();
}

void AApoPlayerController::BeginInactiveState()
{
	Super::BeginInactiveState();

	GetWorld()->GetAuthGameMode()->RestartGame();
}