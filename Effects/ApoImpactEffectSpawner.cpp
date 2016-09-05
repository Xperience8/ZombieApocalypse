// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoImpactEffectSpawner.h"


// Sets default values
AApoImpactEffectSpawner::AApoImpactEffectSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AApoImpactEffectSpawner::SpawnImpactEffect(EPhysicalSurface SurfaceType, FVector SpawnLocation, FRotator SpawnRotation)
{
	auto ParticleSystem = GetImpactParticleSystem(SurfaceType);
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, SpawnLocation, SpawnRotation, true);
	}

	auto SoundCue = GetImpactSoundCue(SurfaceType);
	if (SoundCue)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundCue, SpawnLocation, SpawnRotation);
	}
}

UParticleSystem * AApoImpactEffectSpawner::GetImpactParticleSystem(EPhysicalSurface SurfaceType)
{
	switch (SurfaceType)
	{
	case ZOMBIE_HEAD_SURFACE_TYPE:
	case ZOMBIE_BODY_SURFACE_TYPE:
	case ZOMBIE_LIMB_SURFACE_TYPE:
		return ZombieImpactParticleSystem;
		break;
	}

	return DefaultImpactParticleSystem;
}

USoundCue * AApoImpactEffectSpawner::GetImpactSoundCue(EPhysicalSurface SurfaceType)
{
	switch (SurfaceType)
	{
	case ZOMBIE_HEAD_SURFACE_TYPE:
	case ZOMBIE_BODY_SURFACE_TYPE:
	case ZOMBIE_LIMB_SURFACE_TYPE:
		return ZombieImpactSoundCue;
		break;
	}

	return DefaultImpactSoundCue;
}
