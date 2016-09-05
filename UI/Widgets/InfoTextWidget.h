// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InfoTextWidget.generated.h"

/**
 * Shows info text about focused usable
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UInfoTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Changes info text(only one info text at a time)*/
	UFUNCTION(BlueprintImplementableEvent)
	void SetInfoText(const FString &InfoText);
};
