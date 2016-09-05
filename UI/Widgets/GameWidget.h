// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

class UGameplayWidget;
class UHealthWidget;
class UInfoTextWidget;
class UWeaponWidget;

/**
 * UGameWidget is the root widget which is folded from other widgets
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Shows crosshair*/
	UFUNCTION(BlueprintImplementableEvent)
	void ShowCrosshair();

	/** Hides crosshair*/
	UFUNCTION(BlueprintImplementableEvent)
	void HideCrosshair();

	/** Returns weapon widget*/
	UFUNCTION(BlueprintImplementableEvent)
	UWeaponWidget *GetWeaponWidget();
	
	/** Returns health widget*/
	UFUNCTION(BlueprintImplementableEvent)
	UHealthWidget *GetHealthWidget();

	/** Returns info text widget*/
	UFUNCTION(BlueprintImplementableEvent)
	UInfoTextWidget *GetInfoTextWidget();

	/** Returns gameplay widget*/
	UFUNCTION(BlueprintImplementableEvent)
	UGameplayWidget *GetGameplayWidget();
};
