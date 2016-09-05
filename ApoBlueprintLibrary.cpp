// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoBlueprintLibrary.h"

#include "Characters/Zombie/ApoZombieCharacter.h"

AApoZombieCharacter *UApoBlueprintLibrary::SpawnZombieIntoLevel(UObject *WorldContextObject, const FString &LevelName, TSubclassOf<AApoZombieCharacter> Class, const FTransform &SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, APawn *Instigator, AActor *Owner)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Instigator;

	auto World = GEngine->GetWorldFromContextObject(WorldContextObject);

	// Find level in which we want to spawn new zombie
	// If level doesn't exist, zombie will be spawned in persistent level
	for (auto &StreamingLevel : World->StreamingLevels)
	{
		if (StreamingLevel->IsLevelLoaded())
		{
			if (StreamingLevel->GetWorldAssetPackageName().EndsWith(LevelName))
			{
				SpawnParameters.OverrideLevel = StreamingLevel->GetLoadedLevel();
				break;
			}
		}
	}

	return World->SpawnActor<AApoZombieCharacter>(Class, SpawnTransform, SpawnParameters);
}