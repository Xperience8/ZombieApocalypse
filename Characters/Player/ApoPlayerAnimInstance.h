// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "ApoPlayerAnimInstance.generated.h"

class AApoPlayerCharacter;
class UApoPlayerMovementComponent;

/**
 * 
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UApoPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UApoPlayerAnimInstance() : Owner(nullptr), MovementComponent(nullptr) { }

	/** Cache reference to owner and movement component*/
	virtual void NativeInitializeAnimation() override;

protected:
	/** Returns true if character is shooting*/
	UFUNCTION(BlueprintCallable, Category = "Animation|State")
	bool IsShooting() const;

	/** Returns true if character is aiming*/
	UFUNCTION(BlueprintCallable, Category = "Animation|State")
	bool IsAiming() const;

	/** Returns true if character is moving on the ground(or is staying)*/
	UFUNCTION(BlueprintCallable, Category = "Movement|State")
	bool IsJogging() const;

	/** Returns true if character is jumping(or falling)*/
	UFUNCTION(BlueprintCallable, Category = "Movement|State")
	bool IsJumping() const;

	/** Returns true if character is in crouching state*/
	UFUNCTION(BlueprintCallable, Category = "Movement|State")
	bool IsCrouching() const;

	/** Returns current character moving speed in unreal units(0 means staying)*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentMovingSpeed() const;

	/** Moving angle is difference between forward direction and current moving direction*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentMovingAngle() const;

	/** Returns current controller's pitch rotation in range -90 to 90*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetCurrentPitch() const;


private:
	AApoPlayerCharacter *Owner;
	
	UApoPlayerMovementComponent *MovementComponent;
};
