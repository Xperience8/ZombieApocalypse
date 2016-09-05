// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "ApoPlayerMovementComponent.generated.h"

class AApoPlayerCharacter;

/**
 * UApoPlayerMovementComponent is used for moving player around at specified speed
 */
UCLASS()
class ZOMBIEAPOCALYPSE_API UApoPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UApoPlayerMovementComponent() : MaxAimSpeed(175.f), PlayerCharacter(nullptr) { }

	/** Cache player character*/
	virtual void BeginPlay() override;

	/** Returns current maximum speed in unreal units based on character state*/
	virtual float GetMaxSpeed() const override;

	/** Returns current character speed(works for all moving states) in unreal units*/
	__forceinline float GetCurrentMovingSpeed() const
	{
		return Velocity.Size2D();
	}

	/**
	* Returns current moving orientation(works for all moving states) in degrees
	* Moving angle is difference between forward direction and current moving direction(e.g left)
	*/
	float GetCurrentMovingAngle() const;

private:
	AApoPlayerCharacter *PlayerCharacter;

	/** The maximum speed when character is aiming*/
	UPROPERTY(EditAnywhere)
	float MaxAimSpeed;
};
