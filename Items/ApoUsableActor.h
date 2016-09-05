// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ApoUsableActor.generated.h"

class AApoUsableActor;
class AApoPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSuccessSignature, AApoUsableActor*, Usable, AApoPlayerCharacter*, UsedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFailureSignature, AApoUsableActor*, Usable, AApoPlayerCharacter*, UsedBy);

/**
 * AApoUsableActor is an abstract base class which defines usable objects which can player use
 * It will be highlighted when player is looking on it(call Highlight)
 * It shows info text to player to help him about using specific usables
 */
UCLASS(abstract, HideCategories = ("Input", "Replication", "Actor", "Actor Tick"))
class ZOMBIEAPOCALYPSE_API AApoUsableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AApoUsableActor();

	/** Show lure text and highlight usable*/
	void OnFocus(AApoPlayerCharacter *FocusedBy);

	/** Hide lure text and stop highlighting usable*/
	void OnLostFocus(AApoPlayerCharacter *LostFocusOf);

	/** This method should be overridden in BP to make specific usable effect(e.g. restore health)*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void Use(AApoPlayerCharacter *UsedBy);

	/** You should add implementation to this event in level BP, if you want to make specific effects on successfull use*/
	UPROPERTY(BlueprintAssignable, Category = "Game|Result")
	FOnSuccessSignature OnSuccessEvent;

	/** You should add implementation to this event in level BP, if you want to make specific effects on unsuccessfull use*/
	UPROPERTY(BlueprintAssignable, Category = "Game|Result")
	FOnFailureSignature OnFailureEvent;

protected:
	/** This method should be called when usable was used successfully to inform player about success*/
	UFUNCTION(BlueprintCallable, Category = "Game|Result")
	void OnSuccess(AApoPlayerCharacter *UsedBy);

	/** This method should be called when usable wasn't used successfully to inform player about failure*/
	UFUNCTION(BlueprintCallable, Category = "Game|Result")
	void OnFailure(AApoPlayerCharacter *UsedBy);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *MeshComponent;
	
	/** This sound will be played when usable was successfully used*/
	UPROPERTY(EditAnywhere, Category="Result|Success")
	USoundCue *SuccessSoundCue;

	/** This text will be visibled when usable was successfully used*/
	UPROPERTY(EditAnywhere, Category = "Result|Success")
	FString SuccessInfoText;

	/** This sound will be played when usable was not successfully used*/
	UPROPERTY(EditAnywhere, Category = "Result|Failure")
	USoundCue *FailureSoundCue;
	
	/** This text will be visibled when usable was not successfully used*/
	UPROPERTY(EditAnywhere, Category = "Result|Failure")
	FString FailureInfoText;

	/** This text will be visibled when usable is highlighted but it wasn't used*/
	UPROPERTY(EditAnywhere)
	FString LureInfoText;

	/** It is true when we do not want to destroy usable after it was used*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPermanent;
};
