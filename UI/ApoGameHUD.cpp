// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoGameHUD.h"

#include "UI/Widgets/GameWidget.h"
#include "UI/Widgets/WeaponWidget.h"
#include "UI/Widgets/HealthWidget.h"
#include "UI/Widgets/InfoTextWidget.h"
#include "UI/Widgets/GameplayWidget.h"

AApoGameHUD::AApoGameHUD() : GameWidget(nullptr)
{
	ConstructorHelpers::FClassFinder<UGameWidget> Widget(TEXT("/Game/UI/GameWidgetBlueprint"));
	GameWidgetTemplate = Widget.Class;
}

void AApoGameHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(GameWidgetTemplate);
	GameWidget = CreateWidget<UGameWidget>(GetWorld(), GameWidgetTemplate);
	GameWidget->AddToViewport();
}

void AApoGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GameWidget->RemoveFromViewport();
	GameWidget = nullptr;
}

void AApoGameHUD::UpdateWeaponWidget(int32 NumBullets, int32 NumBulletsInMagazine)
{
	if (!GameWidget)
	{
		return;
	}

	auto WeaponWidget = GameWidget->GetWeaponWidget();
	if (WeaponWidget)
	{
		WeaponWidget->OnNumBulletsChanged(NumBullets, NumBulletsInMagazine);
	}
}

void AApoGameHUD::UpdateHealthWidget(int32 HealthPoints, int32 HealthPointsMax)
{
	if (!GameWidget)
	{
		return;
	}

	auto HealthWidget = GameWidget->GetHealthWidget();
	if (HealthWidget)
	{
		HealthWidget->OnHealthChanged(static_cast<float>(HealthPoints) / static_cast<float>(HealthPointsMax));
	}
}

void AApoGameHUD::ShowCrosshair()
{
	if (!GameWidget)
	{
		return;
	}

	GameWidget->ShowCrosshair();
}

void AApoGameHUD::HideCrosshair()
{
	if (!GameWidget)
	{
		return;
	}
	GameWidget->HideCrosshair();
}

void AApoGameHUD::ShowInfoText(const FString &InfoText, float Duration)
{
	if (!GameWidget)
	{
		return;
	}

	auto InfoTextWidget = GameWidget->GetInfoTextWidget();
	if (InfoTextWidget)
	{
		if (Duration != 0.f)
		{
			GetWorldTimerManager().SetTimer(TemporaryInfoTextTimerHandle, [InfoTextWidget] { InfoTextWidget->SetVisibility(ESlateVisibility::Hidden); }, Duration, false);
		}
		else if (TemporaryInfoTextTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(TemporaryInfoTextTimerHandle);
		}

		InfoTextWidget->SetInfoText(InfoText);
		InfoTextWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AApoGameHUD::HideInfoText()
{
	if (!GameWidget)
	{
		return;
	}

	auto InfoTextWidget = GameWidget->GetInfoTextWidget();
	if (InfoTextWidget && !TemporaryInfoTextTimerHandle.IsValid())
	{
		InfoTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AApoGameHUD::ShowGameplaySummary()
{
	if (!GameWidget)
	{
		return;
	}

	auto GameplayWidget = GameWidget->GetGameplayWidget();
	if (GameplayWidget)
	{
		GameplayWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AApoGameHUD::HideGameplaySummary()
{
	if (!GameWidget)
	{
		return;
	}

	auto GameplayWidget = GameWidget->GetGameplayWidget();
	if (GameplayWidget)
	{
		GameplayWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AApoGameHUD::SetGameplayObjective(const FString &Objective, const FString &Desc)
{
	if (!GameWidget)
	{
		return;
	}

	auto GameplayWidget = GameWidget->GetGameplayWidget();
	if (GameplayWidget)
	{
		GameplayWidget->SetObjective(Objective, Desc);
	}
}

bool AApoGameHUD::IsGameplaySummaryVisibled() const
{
	if (!GameWidget)
	{
		return false;
	}

	auto GameplayWidget = GameWidget->GetGameplayWidget();
	return GameplayWidget ? GameplayWidget->GetVisibility() == ESlateVisibility::HitTestInvisible : false;
}

void AApoGameHUD::ChangeModeToCinema()
{
	if (GameWidget)
	{
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AApoGameHUD::ChangeModeToGame()
{
	if (GameWidget)
	{
		GameWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
