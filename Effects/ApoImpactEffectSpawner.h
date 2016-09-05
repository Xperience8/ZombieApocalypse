// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ApoImpactEffectSpawner.generated.h"

/**
 * AApoImpactEffectSpawner spawns various effects based on surface type(which projectile hit)
 */
UCLASS(abstract)
class ZOMBIEAPOCALYPSE_API AApoImpactEffectSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Turns of tick*/
	AApoImpactEffectSpawner();

	/** Spawns impact effect at specified location for specific surface*/
	void SpawnImpactEffect(EPhysicalSurface SurfaceType, FVector SpawnLocation, FRotator SpawnRotation);

private:
	
	/** Returns impact particle system which should be spawned, when we hit specific surface*/
	UParticleSystem *GetImpactParticleSystem(EPhysicalSurface SurfaceType);

	/** Returns impact sound which should be played, when we hit specific surface*/
	USoundCue *GetImpactSoundCue(EPhysicalSurface SurfaceType);

	/** Default particle system which will be spawned, when we hit some surface which wasn't specified*/
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem *DefaultImpactParticleSystem;

	/** Default sound which will be played, when we hit some surface which wasn't specified*/
	UPROPERTY(EditDefaultsOnly)
	USoundCue *DefaultImpactSoundCue;

	/** Particle system which will be spawned, when we hit zombie*/
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem *ZombieImpactParticleSystem;

	/** Sound which will be played, when we hit zombie*/
	UPROPERTY(EditDefaultsOnly)
	USoundCue *ZombieImpactSoundCue;
	
};
