// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ZombieApocalypseGameMode.h"

#include "Characters/Player/ApoPlayerCharacter.h"
#include "Characters/Player/ApoPlayerController.h"
#include "Gameplay/ApoGameState.h"
#include "UI/ApoGameHUD.h"

AZombieApocalypseGameMode::AZombieApocalypseGameMode()
{
	DefaultPawnClass = AApoPlayerCharacter::StaticClass();
	PlayerControllerClass = AApoPlayerController::StaticClass();
	HUDClass = AApoGameHUD::StaticClass();
}

void AZombieApocalypseGameMode::RestartGame()
{
	// Restart player at saved location
	TActorIterator<AApoPlayerController> PlayerControllerIt(GetWorld());
	if (PlayerControllerIt)
	{
		RestartPlayer(*PlayerControllerIt);
	}

	// Save names of all sublevels which need to be restarted
	for (auto &StreamingLevel : GetWorld()->StreamingLevels)
	{
		if (StreamingLevel->IsLevelLoaded())
		{
			StreamingLevelsToRestart.Add(StreamingLevel);
		}
	}

	StartRestartingStreamingLevels();
}

void AZombieApocalypseGameMode::StartRestartingStreamingLevels()
{
	bool bAllStreamLevelsUnloaded = true;

	// Starts with unloading sublevels, only one sublevel can be unloaded at a time
	// This method will be called again and again, until all sublevels are unloaded
	for (auto StreamingLevel : StreamingLevelsToRestart)
	{
		if (StreamingLevel->IsLevelLoaded())
		{
			FLatentActionInfo ActionInfo;
			ActionInfo.CallbackTarget = this;
			ActionInfo.ExecutionFunction = "StartRestartingStreamingLevels";
			ActionInfo.UUID = 1;
			ActionInfo.Linkage = 0;

			UGameplayStatics::UnloadStreamLevel(this, *GetStreamingLevelName(StreamingLevel), ActionInfo);
			
			bAllStreamLevelsUnloaded = false;
			break;
		}
	}

	// If all sublevels are unloaded, start to load them again
	if (bAllStreamLevelsUnloaded)
	{
		FinishRestartingStreamingLevels();
	}
}

void AZombieApocalypseGameMode::FinishRestartingStreamingLevels()
{
	bool bAllStreamLevelsLoaded = true;

	// Starts with loading sublevels, only one sublevel can be loaded at a time
	// This method will be called again and again, until all sublevels are loaded
	for (auto StreamingLevel : StreamingLevelsToRestart)
	{
		if (!StreamingLevel->IsLevelLoaded())
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading sublevel %s"), *GetStreamingLevelName(StreamingLevel));

			FLatentActionInfo ActionInfo;
			ActionInfo.CallbackTarget = this;
			ActionInfo.ExecutionFunction = "FinishRestartingStreamingLevels";
			ActionInfo.UUID = 1;
			ActionInfo.Linkage = 0;

			UGameplayStatics::LoadStreamLevel(this, *GetStreamingLevelName(StreamingLevel), true, true, ActionInfo);

			bAllStreamLevelsLoaded = false;
			break;
		}
	}

	// If all sublevels are loaded, break restarting process
	if (bAllStreamLevelsLoaded)
	{
		StreamingLevelsToRestart.Empty();
	}
}

AActor * AZombieApocalypseGameMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	// Respawn location is saved inside gamestate
	auto GameState = GetGameState<AApoGameState>();
	if (!GameState || !GameState->GetPlayerStart())
	{
		return Super::FindPlayerStart_Implementation(Player, IncomingName);
	}

	return GameState->GetPlayerStart();
}