// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/GameState.h"
#include "ApoGameState.generated.h"

/**
 * AApoGameState holds all information which are needed for gameplay
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	/** 
	 * Every zombie which is spawned should register yourself, 
	 * so gameplay logic knows how many of them are alive
	 */
	void OnZombieSpawned() 
	{
		++NumLivedZombies;
	}

	/**
	 * Every zombie which is spawned should unregister yourself, 
	 * so gameplay logic knows how many of them are alive
	 */
	void OnZombieDied()
	{
		--NumLivedZombies;
		UE_LOG(LogTemp, Warning, TEXT("Zombies left %d"), NumLivedZombies);
	}

	/** Returns true when specified mission name is the same as currently running mission*/
	UFUNCTION(BlueprintCallable, Category = "Game")
	bool IsMissionRestarted(FString MissionName)
	{
		return MissionName == CurrentMissionName;
	}

	/** 
	 * Saves new spawn location and bookmark name, 
	 * so we know whether we are starting new mission or restarting current one
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartMission(const FString &MissionName, AActor *PlayerRestartStart)
	{
		CurrentMissionName = MissionName;
		PlayerStart = PlayerRestartStart;
	}

	/** Returns saved spawn location, can be null(begin of level -> default spawn location)*/
	AActor *GetPlayerStart() const
	{
		return PlayerStart;
	}
	
protected:
	/** Spawn location which will be used for respawning player*/
	UPROPERTY()
	AActor *PlayerStart;

	/** Name of the current running mission*/
	FString CurrentMissionName;

	/** Number of zombies which are currently alive in the level*/
	UPROPERTY(BlueprintReadOnly)
	int32 NumLivedZombies;
};
