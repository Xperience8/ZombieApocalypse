// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

/**
 * UWeaponWidget shows basic information about weapon which is currently equipped
 */
UCLASS(abstract)
class ZOMBIEAPOCALYPSE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	/**
	* This method should be called after each shot, reload etc.
	* It updates number of bullets and number of bullets in magazine so player stayed informed
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnNumBulletsChanged(int32 NumBullets, int32 NumBulletsInMagazine);

};
