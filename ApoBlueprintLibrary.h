// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApoBlueprintLibrary.generated.h"

class AApoZombieCharacter;

/**
 * Collections of helper methods to make scripting in blueprint easier
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UApoBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** 
	 * Spawns zombie inside specific level(mission)
	 * If level doesn't exist, zombie will be spawned in persistent level
	 */
	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AApoZombieCharacter *SpawnZombieIntoLevel(UObject *WorldContextObject, const FString &LevelName, TSubclassOf<AApoZombieCharacter> Class, const FTransform &SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, APawn *Instigator = nullptr, AActor *Owner = nullptr);

};
