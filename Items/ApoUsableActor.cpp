// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoUsableActor.h"

#include "Characters/Player/ApoPlayerCharacter.h"
#include "UI/ApoGameHUD.h"

// Sets default values
AApoUsableActor::AApoUsableActor() : LureInfoText("Lure text was not set, please set it in editor"),
	SuccessInfoText("Success text was not set, please set it in editor"),
	FailureInfoText("Failure text was not set, please set it in editor"), bIsPermanent(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void AApoUsableActor::OnFocus(AApoPlayerCharacter* FocusedBy)
{
	MeshComponent->SetRenderCustomDepth(true);

	auto GameHUD = FocusedBy->GetGameHUD();
	if (GameHUD)
	{
		GameHUD->ShowInfoText(LureInfoText);
	}
}

void AApoUsableActor::OnLostFocus(AApoPlayerCharacter* LostFocusOf)
{
	MeshComponent->SetRenderCustomDepth(false);
	
	auto GameHUD = LostFocusOf->GetGameHUD();
	if (GameHUD)
	{
		GameHUD->HideInfoText();
	}
}

void AApoUsableActor::OnSuccess(AApoPlayerCharacter *UsedBy)
{
	// Play sound so player knews, that usable was used
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuccessSoundCue, GetActorLocation());

	auto GameHUD = UsedBy->GetGameHUD();
	if (GameHUD)
	{
		GameHUD->ShowInfoText(SuccessInfoText, bIsPermanent ? 0.f : 3.f);
	}

	if (!bIsPermanent)
	{
		Destroy();
	}

	OnSuccessEvent.Broadcast(this, UsedBy);
}

void AApoUsableActor::OnFailure(AApoPlayerCharacter *UsedBy)
{
	// Play sound so player knews, that usable was not used
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FailureSoundCue, GetActorLocation());
	
	auto GameHUD = UsedBy->GetGameHUD();
	if (GameHUD)
	{
		GameHUD->ShowInfoText(FailureInfoText);
	}

	OnFailureEvent.Broadcast(this, UsedBy);
}
