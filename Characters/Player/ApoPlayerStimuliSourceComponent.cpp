// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoPlayerStimuliSourceComponent.h"

UApoPlayerStimuliSourceComponent::UApoPlayerStimuliSourceComponent(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer), TimeToSendNoiseEvent(0.5f)
{
	RegisterAsSourceForSenses.Add(UAISense_Hearing::StaticClass());
	RegisterAsSourceForSenses.Add(UAISense_Sight::StaticClass());
}

void UApoPlayerStimuliSourceComponent::RegisterPawnAsSource(APawn* Pawn)
{
	if (NoiseEvent.Instigator && NoiseEvent.Instigator == Pawn)
	{
		return;
	}

	NoiseEvent.Instigator = Pawn;
	NoiseEvent.Age = 0.f;

	RegisterWithPerceptionSystem();

	GetWorld()->GetTimerManager().SetTimer(NoiseEventTimerHandle, this, &UApoPlayerStimuliSourceComponent::SendPlayerNoise, TimeToSendNoiseEvent, true);
}

void UApoPlayerStimuliSourceComponent::UnregisterPawnAsSource()
{
	NoiseEvent.Instigator = nullptr;

	UnregisterFromPerceptionSystem();
	
	GetWorld()->GetTimerManager().ClearTimer(NoiseEventTimerHandle);
}

void UApoPlayerStimuliSourceComponent::MakeNoise(FVector NoiseLocation, float NoiseRange, float NoiseLoudness, FName NoiseTag)
{
	bool bOverrideNoise = true;

	// If we have already registered another noise, we must check whether is stronger
	// or it is the same noise as registered, if it is the same then will be updated(newer location data)
	if (NoiseEvent.Age != 0.f && NoiseTag != NoiseEvent.Tag)
	{
		// Compute factors for checking how strong noise is
		float NewNoiseFactor = NoiseLoudness * NoiseRange;
		float OldNoiseFactor = NoiseEvent.Loudness * NoiseEvent.MaxRange;

		// We have new sound, but it is not louder than previous, so discard it
		if (NewNoiseFactor < OldNoiseFactor)
		{
			bOverrideNoise = false;
		}
	}
	
	// If we don't have registered any noise or the previos noise was weaker
	if (bOverrideNoise)
	{
		NoiseEvent.NoiseLocation = NoiseLocation;
		NoiseEvent.MaxRange = NoiseRange;
		NoiseEvent.Tag = NoiseTag;
		NoiseEvent.Age += 1.f;
	}
}

void UApoPlayerStimuliSourceComponent::SendPlayerNoise()
{
	// If player made at least one noise 
	if (NoiseEvent.Age > 0.f)
	{
		// We use age as a counter for how many noises were made,
		// but API use this value differently, therefore we set here
		// zero to tell API that this is the most recent noise event
		NoiseEvent.Age = 0.f;

		UAIPerceptionSystem::OnEvent(GetWorld(), NoiseEvent);
	}
}
