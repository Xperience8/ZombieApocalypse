// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * UGameplayWidget shows information to player about current mission
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Changes current objective, which is shown to player(only one objective at a time)*/
	UFUNCTION(BlueprintImplementableEvent)
	void SetObjective(const FString &Objective, const FString &Desc);
};
