// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ApoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AApoPlayerController();

	/** Restart game(player died)*/
	virtual void BeginInactiveState() override;
};
