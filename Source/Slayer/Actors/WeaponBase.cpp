// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/WeaponBase.h"

#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Animation/AnimInstance/AnimInst.h"
#include "Slayer/Components/CombatComponent.h"
#include "Slayer/Components/CollisionComponent.h"

AWeaponBase::AWeaponBase()
	: CombatType(ECombatType::ECT_None), WeaponDrawMontage(nullptr), WeaponSheathMontage(nullptr)
{
	UnequippedSocketName = "unequipped_hip_soc";
	EquippedSocketName = "weapon_light_soc";

	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("Collision Component"));

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnHit.AddDynamic(this, &AWeaponBase::OnHit);
}

void AWeaponBase::OnEquip()
{
	Super::OnEquip();

	SetIsEquipped(true);
	CombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
	ACharacterBase* OwningChar = Cast<ACharacterBase>(GetOwner());
	UAnimInstance* AnimInst = OwningChar->GetMesh()->GetAnimInstance();

	if (!CombatComponent || !AnimInst){
		return;
	}


	if (CombatComponent->IsCombatEnabled())
	{
		AttachActor(EquippedSocketName);
	}
	else
	{
		AttachActor(UnequippedSocketName);
	}
	
	CombatComponent->SetMainWeapon(this);

	bool bIsUsingAnimInterface = false;
	bIsUsingAnimInterface = AnimInst->Implements<UAnimInstanceInterface>();

	if (bIsUsingAnimInterface)
	{
		IAnimInstanceInterface::Execute_UpdateCombatType(AnimInst, CombatType);
	}

	CollisionComponent->SetCollisionMesh(GetItemMesh());
	CollisionComponent->AddActorToIgnore(this);
	CollisionComponent->AddActorToIgnore(GetOwner());
}

void AWeaponBase::OnUnequip()
{

}

void AWeaponBase::OnHit(FHitResult Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("Hit Detected")));
}
