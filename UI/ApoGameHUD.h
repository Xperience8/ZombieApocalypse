// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "ApoGameHUD.generated.h"

class UGameWidget;

/**
 * AApoGameHUD shows and mantain HUD which is visible to player
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AApoGameHUD();

	/** Creates default game widget*/
	virtual void PostInitializeComponents() override;

	/** Destroys default game widget*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Updates weapon widget with new information about weapon(e.g number of bullets)*/
	void UpdateWeaponWidget(int32 NumBullets, int32 NumBulletsInMagazine);

	/** Updates health widget with new information about health(e.g current health change)*/
	void UpdateHealthWidget(int32 HealthPoints, int32 HealthPointsMax);

	/** Sets crosshair visibility to true*/
	void ShowCrosshair();

	/** Sets crosshair visibility to false*/
	void HideCrosshair();

	/** Sets and shows new info text*/
	void ShowInfoText(const FString &InfoText, float Duration = 0.f);

	/** Delete and hide old info text*/
	void HideInfoText();

	/** Shows gameplay summary with current objective*/
	void ShowGameplaySummary();

	/** Hides gameplay summary, current objective will state same*/
	void HideGameplaySummary();

	/** Changes current objective*/
	void SetGameplayObjective(const FString &Objective, const FString &Desc);

	/** Returns true if gameplay summary is visibled*/
	bool IsGameplaySummaryVisibled() const;

	/** Hides all widgets*/
	UFUNCTION(BlueprintCallable, Category="UI")
	void ChangeModeToCinema();

	/** Shows all default widgets, that are needed for basic gameplay*/
	UFUNCTION(BlueprintCallable, Category="UI")
	void ChangeModeToGame();

private:
	/** Shows default HUD to player(e.g. crosshair, healthbar etc.)*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameWidget> GameWidgetTemplate;

	/** Shows default HUD to player(e.g. crosshair, healthbar etc.)*/
	UGameWidget *GameWidget;

	FTimerHandle TemporaryInfoTextTimerHandle;
};
