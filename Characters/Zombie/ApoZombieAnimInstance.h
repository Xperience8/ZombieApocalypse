// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "ApoZombieAnimInstance.generated.h"

class AApoZombieCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UApoZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UApoZombieAnimInstance() : Owner(nullptr), MovementComponent(nullptr) { }

	/** Cache reference to owner and movement component*/
	virtual void NativeInitializeAnimation() override;

protected:	
	/** Returns true if character is moving on the ground(or is staying)*/
	UFUNCTION(BlueprintCallable, Category = "Movement|State")
	bool IsWalking() const;

	/** Returns current character moving speed in unreal units(0 means staying)*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentMovingSpeed() const;

private:
	AApoZombieCharacter *Owner;

	UCharacterMovementComponent *MovementComponent;
};
