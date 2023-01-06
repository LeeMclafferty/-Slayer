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

	SetIsEquipped(true);
	UCombatComponent* CombatComponent = GetOwner()->GetComponentByClass<UCombatComponent>();
	UAnimInstance* AnimInst = OwningChar->GetMesh()->GetAnimInstance();

	if (!OwningChar || !AnimInst){
		return;
	}


	if (OwningChar->IsCombatEnabled())
	{
		AttachActor(EquippedSocketName);
	}
	else
	{
		AttachActor(UnequippedSocketName);
	}
	
	OwningChar->SetMainWeapon(this);

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
