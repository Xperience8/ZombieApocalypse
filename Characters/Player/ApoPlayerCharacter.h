// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/ApoCharacter.h"
#include "ApoPlayerCharacter.generated.h"


class AApoGameHUD;
class AApoUsableActor;
class AApoWeapon;
class UApoInventoryComponent;
class UApoPlayerStimuliSourceComponent;
class UInputComponent;

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoPlayerCharacter : public AApoCharacter
{
	GENERATED_BODY()
public:
	explicit AApoPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/** Prepares player to begin play(initialize HUD and register as source of stimuli)*/
	virtual void BeginPlay() override;

	/** Check wheter we are not looking at usable and create noise if we are moving*/
	virtual void Tick(float DeltaSeconds) override;

	/** Unregister as source of stimuli*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Called to bind functionality to input*/
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Take damage and inform HUD about current health*/
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Kills character and changes his state to corpse(everything is blocked, character is laying and waiting for restart)*/
	virtual void Die(struct FDamageEvent const& DamageEvent) override;

	/** Returns true if equipped weapon is shooting*/
	bool IsShooting() const;

	/** Returns true if player is aiming*/
	bool IsAiming() const
	{
		return bAiming;
	}

	/** Returns true if player has full HP*/
	UFUNCTION(BlueprintCallable, Category="Stats")
	bool HasFullHealthPoints() const
	{
		return HealthPoints == HealthPointsMax;
	}

	/** Restores health points and updates HUD*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void RestoreHealthPoints(float RestoreCoef);

	/** Changes current gameplay objective and showsg gameplay summary*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void UpdateGameplayObjective(const FString &Objective, const FString &Description);

	/** Returns equipped weapon(can not be null)*/
	UFUNCTION(BlueprintCallable, Category = "Gadgets")
	AApoWeapon *GetEquippedWeapon() const;

	/** Returns game HUD(can be null)*/
	AApoGameHUD *GetGameHUD();

	/** Returns inventory of this actor, it can not be null */
	UApoInventoryComponent *GetInventoryComponent()
	{
		return InventoryComponent;
	}

	/** Returns stimuli source component which should be used to create stimuli which are sensed by zombies*/
	UApoPlayerStimuliSourceComponent *GetStimuliSourceComponent()
	{
		return StimuliSourceComponent;
	}

private:
	/** Returns usable actor(if exist) on who we are looking at(short distance)*/
	AApoUsableActor *GetUsableActorInView();

	/** Moves character forward, negative Axis value moves character backward*/
	void MoveForward(float Axis);

	/** Moves character right, negative Axis value moves character left*/
	void MoveRight(float Axis);

	/** Adds pitch rotation to controller*/
	void LookUp(float Axis);

	/** Adds yaw rotation to controller*/
	void Turn(float Axis);

	/** Player performs single jump if he is moving forward or isn't moving at all*/
	virtual void Jump() override;

	/** Player begins crouching*/
	void OnCrouch();

	/** Player ends crouching*/
	void OnUnCrouch();

	/** Start shooting if player has weapon*/
	void OnStartShooting();

	/** Stop shooting*/
	void OnStopShooting();

	/** Reload equipped weapon*/
	void OnReload();

	/** Player starts aim*/
	void OnStartAiming();

	/** Player stops aim*/
	void OnStopAiming();

	/** Player uses usable actor(e.g ammo pickup), if usable exists, its nearby and can be used*/
	void OnUseUsable();

	void OnShowCurrentObjective();

	void Quit();

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *ThridPersonArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *ThirdPersonCameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *DeathArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *DeathCameraComponent;

	UPROPERTY(VisibleAnywhere)
	UApoInventoryComponent *InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	UApoPlayerStimuliSourceComponent *StimuliSourceComponent;

	UPROPERTY(EditAnywhere)
	float UsableActorMaxDistance;

	AApoUsableActor *UsableActorInView;

	bool bAiming;
};
