// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * UHealthWidget shows information about health system
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Changes health, HealthCoef is CurrentHealth / MaxHealth in range 0.0 to 1.0*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float HealthCoef);
};
