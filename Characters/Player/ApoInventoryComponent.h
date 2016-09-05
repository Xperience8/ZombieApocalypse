// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "ApoInventoryComponent.generated.h"

class AApoWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIEAPOCALYPSE_API UApoInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UApoInventoryComponent();

	/** Spawn and attach weapon*/ 
	virtual void BeginPlay() override;

	/** Destroy and detach weapon*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/*** Returns equipped weapon*/
	AApoWeapon *GetEquippedWeapon()
	{
		return EquippedWeapon;
	}

private:
	/** Template which is used to create primary weapon*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AApoWeapon> WeaponTemplate;

	UPROPERTY(EditAnywhere)
	FName WeaponSocketName;

	AApoWeapon *EquippedWeapon;
};
