// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/WeaponBase.h"

#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Animation/AnimInstance/AnimInst.h"
#include "Slayer/Components/CombatComponent.h"

AWeaponBase::AWeaponBase()
	: CombatType(ECombatType::ECT_None), WeaponDrawMontage(nullptr), WeaponSheathMontage(nullptr)
{
	UnequippedSocketName = "unequipped_hip_soc";
	EquippedSocketName = "weapon_light_soc";
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
}

void AWeaponBase::OnUnequip()
{

}
