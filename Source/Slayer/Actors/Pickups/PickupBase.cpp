// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/Pickups/PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Actors/EquippableBase.h"

//Make sure to set ItemClass in BP
APickupBase::APickupBase()
	:ItemClass(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName("Interactable");
	SphereCollision->SetSphereRadius(100.f);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

}


void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::Interact_Implementation(AActor* Caller)
{
	SpawnPickupActor(Caller);
}

/* This system is only good for pickups that spawn a physical actor, I will have to rework it to do other things
like add to inventory.*/
void APickupBase::SpawnPickupActor(AActor* Caller)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caller;
	SpawnParams.Instigator = Cast<APawn>(Caller);

	auto SpawnedItem = GetWorld()->SpawnActor<AEquippableBase>(ItemClass, Caller->GetActorLocation(), Caller->GetActorRotation(), SpawnParams);

	if (SpawnedItem)
	{
		SpawnedItem->OnEquip();
	}
}

