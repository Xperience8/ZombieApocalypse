// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoZombieController.h"

#include "ApoZombieCharacter.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>

#include "Characters/Player/ApoPlayerController.h"
#include "Characters/Player/ApoPlayerCharacter.h"

AApoZombieController::AApoZombieController() : HeightDifferenceInBuilding(200)
{
	auto PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AApoZombieController::PerceptionUpdated);
	
	auto HearingSense = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing sense");
	HearingSense->HearingRange = 3000.f;
	HearingSense->DetectionByAffiliation.bDetectEnemies = true;
	HearingSense->DetectionByAffiliation.bDetectNeutrals = true;
	HearingSense->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*HearingSense);

	SetPerceptionComponent(*PerceptionComponent);
}

void AApoZombieController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	auto Zombie = Cast<AApoZombieCharacter>(InPawn);
	if (Zombie && Zombie->GetBehaviorTree())
	{
		RunBehaviorTree(Zombie->GetBehaviorTree());

		if (Blackboard)
		{
			NoiseLocationKey = Blackboard->GetKeyID("NoiseLocation");
		}
	}
}

void AApoZombieController::UnPossess()
{
	Super::UnPossess();

	auto Brain = GetBrainComponent();
	if (Brain)
	{
		Brain->StopLogic("Zombie is dead");
	}
}

void AApoZombieController::PerceptionUpdated(TArray<AActor*> UpdatedActors)
{
	for (AActor *UpdatedActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		GetPerceptionComponent()->GetActorsPerception(UpdatedActor, PerceptionInfo);
		for (FAIStimulus &Stimulus : PerceptionInfo.LastSensedStimuli)
		{
			if (Stimulus.Type == UAISense_Hearing::GetSenseID<UAISense_Hearing>())
			{
				auto Zombie = Cast<AApoZombieCharacter>(GetPawn());

				// Disable sounds which comes from different floors
				float HeightDifference = FMath::Abs(Stimulus.StimulusLocation.Z - Zombie->GetActorLocation().Z);
				if (HeightDifference < HeightDifferenceInBuilding)
				{
					Blackboard->SetValue<UBlackboardKeyType_Vector>(NoiseLocationKey, Stimulus.StimulusLocation);
				}
			}
			else if (Stimulus.Type == UAISense_Sight::GetSenseID<UAISense_Sight>())
			{
				Stimulus.SetWantsNotifyOnlyOnValueChange(true);
				if (Stimulus.WasSuccessfullySensed())
				{
					Blackboard->SetValueAsObject("SensedPlayer", UpdatedActor);
				}
				// If we lost player from the sight, cached his last known position
				else if (Blackboard->GetValueAsObject("SensedPlayer") != nullptr)
				{
					Blackboard->SetValueAsObject("SensedPlayer", nullptr);
					Blackboard->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
				}
			}
		}
	}
}

ETeamAttitude::Type AApoZombieController::GetTeamAttitudeTowards(const AActor & Other) const
{
	return Cast<const AApoPlayerCharacter>(&Other) ? ETeamAttitude::Type::Hostile : ETeamAttitude::Type::Friendly;
}
