// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "ApoZombieController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoZombieController : public AAIController
{
	GENERATED_BODY()

public:
	AApoZombieController();
	
	/** Start behavior tree*/
	virtual void Possess(APawn* InPawn) override;

	/** Stop behavior tree*/
	virtual void UnPossess() override;

	/** Solve stimuli*/
	UFUNCTION()
	void PerceptionUpdated(TArray<AActor*> UpdatedActors);

	/** Retrieved owner attitude toward given Other object */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
private:
	FBlackboard::FKey NoiseLocationKey;
	
	/** Ignore noise which comes from above or below(in buildings)*/
	UPROPERTY(EditAnywhere)
	float HeightDifferenceInBuilding;
};
