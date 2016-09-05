// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/ApoCharacter.h"
#include "ApoZombieCharacter.generated.h"

class AApoPlayerCharacter;
class UBehaviorTree;

UENUM(BlueprintType)
enum class EZombieState
{
	None UMETA(DisplayName = "None"),
	Idle UMETA(DisplayName = "Idle"),
	Wandering UMETA(DisplayName = "Wandering"),
	Hunting UMETA(DisplayName = "Hunting")
};

/**
 * AApoZombieCharacter is an enemy for player
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoZombieCharacter : public AApoCharacter
{
	GENERATED_BODY()
	
public:
	AApoZombieCharacter(const FObjectInitializer& ObjectInitializer);

	/** Register yourself with gameplay system*/
	virtual void BeginPlay() override;

	/** Unregister yourself from gameplay systemS*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Subtract damage from health points*/
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Kills character, activates rigid body simulation and changes his state to corpse(everything is blocked, character is laying and waiting for restart)*/
	virtual void Die(struct FDamageEvent const& DamageEvent) override;

	/** Returns true if player is in attackable radius, enough time has passed and is in front of zombie*/
	UFUNCTION(BlueprintCallable, Category = "AI|Behavior|Attack")
	bool CanAttack(AApoPlayerCharacter *PlayerToAttack) const;

	/** Makes damage to player if CanAttack returns true*/
	UFUNCTION(BlueprintCallable, Category = "AI|Behavior|Attack")
	void Attack(AApoPlayerCharacter *PlayerToAttack);

	/** Changes type of noise this zombie produces(e.g idle, hunting)*/
	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void ChangeZombieState(EZombieState ZombieState);

	__forceinline UBehaviorTree *GetBehaviorTree()
	{
		return BehaviorTree;
	}

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent *ZombieNoiseComponent;

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue *IdleSoundCue;

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue *WanderingSoundCue;

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue *HuntingSoundCue;

	UPROPERTY(EditDefaultsOnly, Category="Behavior")
	UBehaviorTree *BehaviorTree;

	/** Damage that produces zombie on attack*/
	UPROPERTY(EditAnywhere, Category="Behavior|Attack")
	float AttackDamage;

	/** How many seconds should zombie wait for next attack*/
	UPROPERTY(EditAnywhere, Category="Behavior|Attack")
	float DelayBetweenAttacks;

	/** How far away can be player to successfully attack him*/
	UPROPERTY(EditAnywhere, Category = "Behavior|Attack")
	float MaxDistanceToAttack;

	/** This factor will be multiplied by weapon damage if head was shot*/
	UPROPERTY(EditAnywhere, Category="DamageModifiers")
	float HeadDamageFactor;

	/** This factor will be multiplied by weapon damage if body was shot*/
	UPROPERTY(EditAnywhere, Category = "DamageModifiers")
	float BodyDamageFactor;

	/** This factor will be multipled by weapon damage if limb was shot*/
	UPROPERTY(EditAnywhere, Category = "DamageModifiers")
	float LimbDamageFactor;

	float LastAttackTime;

	EZombieState CurrentState;
};
