// Copyright 2016 Patrick Jurasek. All Rights Reserved.

#include "ZombieApocalypse.h"
#include "ApoInventoryComponent.h"

#include "Weapons/ApoWeapon.h"

// Sets default values for this component's properties
UApoInventoryComponent::UApoInventoryComponent() : WeaponSocketName("WeaponSocket"), EquippedWeapon(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UApoInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get weapon socket from owner
	auto Owner = static_cast<ACharacter*>(GetOwner());
	auto OwnerMesh = Owner->GetMesh();
	auto WeaponSocket = OwnerMesh->GetSocketByName(WeaponSocketName);
	if (WeaponTemplate && WeaponSocket)
	{
		// Spawn weapon and attach it to socket
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = Owner;
		SpawnParameters.Owner = Owner;

		EquippedWeapon = GetWorld()->SpawnActor<AApoWeapon>(WeaponTemplate, SpawnParameters);
		WeaponSocket->AttachActor(EquippedWeapon, OwnerMesh);
		EquippedWeapon->OnEquip();
	}
}

void UApoInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->Destroy();
	}
}
