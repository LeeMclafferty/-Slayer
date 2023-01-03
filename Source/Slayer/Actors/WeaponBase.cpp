// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/WeaponBase.h"

#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Animation/AnimInstance/AnimInst.h"

AWeaponBase::AWeaponBase()
	: CombatType(ECombatType::ECT_None), WeaponDrawMontage(nullptr), WeaponSheathMontage(nullptr)
{
	AttachSocketName = "unequipped_hip_soc";
	HandSocketName = "weapon_r_soc";
}

void AWeaponBase::OnEquip()
{
	Super::OnEquip();
	
	ACharacterBase* OwningChar = Cast<ACharacterBase>(GetOwner());
	UAnimInstance* AnimInst = OwningChar->GetMesh()->GetAnimInstance();

	if (!OwningChar || !AnimInst){
		return;
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

void AWeaponBase::SetAttachedToHand(bool NewAttachment)
{
	bIsAttachedToHand = NewAttachment;

	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		UAnimInstance* AnimInst = OwnerChar->GetMesh()->GetAnimInstance();
		bool bIsUsingAnimInterface = false;
		bIsUsingAnimInterface = AnimInst->Implements<UAnimInstanceInterface>();

		if (bIsUsingAnimInterface)
		{
			IAnimInstanceInterface::Execute_UpdateWeaponAttachedToHand(AnimInst, bIsAttachedToHand);
		}
	}
}
