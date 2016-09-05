// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ApoWeapon.generated.h"

UENUM()
enum EApoWeaponState
{
	Unequipped,
	Idle,
	Shooting,
	Reloading
};

/** 
 * AApoWeapon is a weapon which can be attached to player(equip)
 * It can shoot projectiles, which damage target(instantly)
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API AApoWeapon : public AActor
{
	GENERATED_BODY()
public:	

	AApoWeapon();

	/** If weapon is in shooting state, shoot if enough time has passed, otherwise do nothing*/
	virtual void Tick( float DeltaSeconds ) override;

	/** 
	 * Starts shooting if there is enought ammo in magazine, if it's not, perform reload
	 */
	void StartShooting();

	/** Stops shooting*/
	void StopShooting();

	/** 
	 * Reload weapon and play animation
	 * It is safe to call this method even if player is shooting
	 * Weapon states are updated in FinishReload method(after animation)
	 */
	void Reload();

	/** Reload will be canceled*/
	void InterruptReload();

	/** Restores ammo in this weapon to maximum capacity(reload is performed)*/
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void RestoreAmmoFull();

	/** Restores specified number of magazines(no reload)*/
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void RestoreAmmoInMagazines(int32 NumMagazines);

	/** Perform reload and inform player about this weapon(no animation of reloading)*/
	__forceinline void OnEquip()
	{
		WeaponState = EApoWeaponState::Idle;
		FinishReload();
	}

	/** Do not inform player about this weapon and block this weapon(no shooting, reloading etc.)*/
	__forceinline void OnUnequip()
	{
		WeaponState = EApoWeaponState::Unequipped;
	}

	/** Returns true if player is shooting*/
	__forceinline bool IsShooting() const
	{
		return WeaponState == EApoWeaponState::Shooting;
	}

	/** Returns true if player is reloading*/
	__forceinline bool IsReloading() const
	{
		return WeaponState == EApoWeaponState::Reloading;
	}

	/** Returns true if this weapon is equipped*/
	__forceinline bool IsEquipped() const
	{
		return WeaponState != EApoWeaponState::Unequipped;
	}

	/** Returns true if this weapon can not take more ammo*/
	UFUNCTION(BlueprintCallable, Category="Ammo")
	bool IsAmmoFull() const
	{
		return NumBullets + NumBulletsInMagazine == NumBulletsMax + NumBulletsPerMagazine;
	}

	/** Returns true when magazine is empty(there are no bullets in magazine -> reload is needed)*/
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool IsEmpty() const
	{
		return NumBulletsInMagazine == 0;
	}

	/** Quick hack to set ammo in bullets(e.g at startup)*/
	UFUNCTION(BlueprintCallable, Category = "Hack")
	void SetAmmoInBullets(int32 BulletsInBackpack, int32 BulletsInMagazine, bool bReload = false);

	/** Quick hack to set ammo in magazines(e.g at startup)*/
	UFUNCTION(BlueprintCallable, Category = "Hack")
	void SetAmmoInMagazines(int32 NumMagazines, bool bReload = false);

	/** Returns current number of bullets in backpack*/
	__forceinline int32 GetNumBullets() const
	{
		return NumBullets;
	}

	/** Returns current number of bullets in magazine*/
	__forceinline int32 GetNumBulletsInMagazine() const
	{
		return NumBulletsInMagazine;
	}

protected:
	/** Enable shooting and start playing shooting sound*/
	void EnableShooting();

	/** Disable shooting and stop playing shooting sound*/
	void DisableShooting();

	/**
	 * Shoot one bullet to the target and inform player about it
	 * It is safe to call this method when no bullets are in magazine -> initiate reload
	 */
	void Shoot();

	/** Appropriate effect will be created based on what we hit*/
	void SpawnImpactEffect(FHitResult HitResult);

	/**
	 * This method should be called when reload animation is done
	 * It's main job is to recompute bullets(put inside magazine and remove from heap),
	 * enable shooting if it was disabled before reloading and inform player
	 */
	void FinishReload();

	/** Shot bullet and return whether we hit something*/
	FHitResult TraceShot(FVector Start, FVector AimDir);

	/** Inform player about new weapon state*/
	void InformOwner();

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	UAudioComponent *ShootingAudioComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *ShootingParticleSystemComponent;

	/** Sound cue which is played when player hasn't any bullets*/
	UPROPERTY(EditAnywhere)
	USoundCue *NoBulletsSoundCue;

	/** Animation which is played when player is reloading*/
	UPROPERTY(EditAnywhere)
	UAnimMontage *ReloadAnimMontage;

	/** Timer which calls event after reload animation is done*/
	FTimerHandle ReloadTimerHandle;

	/** How many bullets can have this gun at max*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	int32 NumBulletsMax;

	/** How many bullets can be put inside magazine*/
	UPROPERTY(EditAnywhere, Category = "Ammunition")
	int32 NumBullets;

	/** How many bullets can be in magazine(max)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	int32 NumBulletsPerMagazine;

	/** How many bullets are in current magazine*/
	UPROPERTY(EditAnywhere, Category = "Ammunition")
	int32 NumBulletsInMagazine;

	/** Maximum distance that bullet can travel*/
	UPROPERTY(EditAnywhere)
	float MaxShotDistance;

	/** How many seconds should passed between shots*/
	UPROPERTY(EditAnywhere)
	float DelayBetweenShots;

	/** Base damage which produces single shot(this damage will be scaled by part of the body we've hit)*/
	UPROPERTY(EditAnywhere)
	float ShotDamage;

	/** When was the last shot fired*/
	float LastShotTime;

	/** It is true when player wants to shoot*/
	bool bWantsToShoot;

	EApoWeaponState WeaponState;
};
