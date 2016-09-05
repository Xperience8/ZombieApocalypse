// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ApoPlayerStimuliSourceComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UApoPlayerStimuliSourceComponent : public UAIPerceptionStimuliSourceComponent
{
	GENERATED_BODY()
public:
	explicit UApoPlayerStimuliSourceComponent(const FObjectInitializer &ObjectInitializer);
	
	/** Register pawn as a source for stimuli*/
	void RegisterPawnAsSource(APawn *Pawn);
	
	/** Unregister currently possessed pawn as a source for stimuli*/
	void UnregisterPawnAsSource();

	/** Register noise and if it is stronger than previous noise, it will be cached and later send*/
	void MakeNoise(FVector NoiseLocation, float NoiseRange, float NoiseLoudness, FName NoiseTag);

private:
	/** This method will be repeatedly called to inform zombies about player noise*/
	void SendPlayerNoise();

	/** How often will be noise event sent*/
	UPROPERTY(EditAnywhere)
	float TimeToSendNoiseEvent;

	FTimerHandle NoiseEventTimerHandle;

	/** The strongest noise event is saved, which will be send*/
	FAINoiseEvent NoiseEvent;
};
