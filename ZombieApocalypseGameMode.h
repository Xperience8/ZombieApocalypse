// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "ZombieApocalypseGameMode.generated.h"

/**
 * It's main job is to restart game without reloading persistent level
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AZombieApocalypseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	/** Set default classes for spawning player*/
	AZombieApocalypseGameMode();

	/** Restarts every loaded sublevel and respawn player at the saved location*/
	virtual void RestartGame() override;

	/** Unloads all streaming levels which are inside StreamingLevelsToRestart array*/
	UFUNCTION()
	void StartRestartingStreamingLevels();

	/** Loads all streaming levels which are inside StreamingLevelsToRestart array*/
	UFUNCTION()
	void FinishRestartingStreamingLevels();

	/** Return player's checkpoint location*/
	virtual AActor *FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

private:
	/** Removes path from level name*/
	FString GetStreamingLevelName(ULevelStreaming* StreamingLevel)
	{
		// Remove path from level name
		FString LevelName = StreamingLevel->PackageNameToLoad.ToString();
		int32 Index = 0;
		LevelName.FindLastChar('/', Index);
		return LevelName.RightChop(Index + 1);
	}

	/** Array of sublevels which will be restarted on players death*/
	TArray<ULevelStreaming*> StreamingLevelsToRestart;
};
