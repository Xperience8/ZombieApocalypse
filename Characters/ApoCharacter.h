// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "ApoCharacter.generated.h"

UCLASS(abstract)
class ZOMBIEAPOCALYPSE_API AApoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit AApoCharacter(const FObjectInitializer& ObjectInitializer);

	/** Subtract damage from health and play sound*/
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Plays rigid body simulation based on damage event and changes state to corpse*/
	virtual void Die(struct FDamageEvent const& DamageEvent);

	/** Returns true if character is dying or death(corpse)*/
	bool IsDying() const
	{
		return bDying;
	}

protected:
	/** Activate ragdoll physics to simulate dying*/
	void ActivateRagdollPhysics();

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue *HitSoundCue;

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue *DeathSoundCue;

	/** Maximum number of health points which can character have*/
	UPROPERTY(EditAnywhere, Category="Stats")
	int32 HealthPointsMax;

	/** Current number of health point which character has*/
	UPROPERTY(EditAnywhere, Category="Stats")
	int32 HealthPoints;

	/** How many seconds should passed to dissappear character's corpse(including dying)*/
	UPROPERTY(EditAnywhere, Category="Dying")
	float TimeAsCorpse;

	bool bDying;
};
