// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/EquippableBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

AEquippableBase::AEquippableBase()
	:UnequippedSocketName("unequipped_hip_soc"), EquippedSocketName("weapon_r_soc"), bIsEquipped(false)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetupAttachment(RootComponent);
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEquippableBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEquippableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

class UPrimitiveComponent* AEquippableBase::GetItemMesh()
{
	if (ItemStaticMesh)
	{
		return ItemStaticMesh;
	}
	else
	{
		return ItemSkeletalMesh;
	}
}

void AEquippableBase::OnEquip()
{
	bIsEquipped = true;

	AttachActor(UnequippedSocketName);
}

void AEquippableBase::OnUnequip()
{
	if (bIsEquipped)
	{
		bIsEquipped = false;
	}
}

void AEquippableBase::AttachActor(FName SocketName)
{
	ACharacter* OwningChar = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* SkMesh = OwningChar->GetMesh();

	if(!OwningChar || !SkMesh){ 
		return;
	}

	AttachToComponent(SkMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}


