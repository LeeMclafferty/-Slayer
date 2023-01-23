// Fill out your copyright notice in the Description page of Project Settings.

#include "Slayer/Components/CombatComponent.h"

#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Animation/AnimInstance/AnimInst.h"

UCombatComponent::UCombatComponent()
	:AttackCount(0), MainWeapon(nullptr), bIsCombatEnabled(false), 
	bWasAttackedSaved(false)
{
	PrimaryComponentTick.bCanEverTick = true;

}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetMainWeapon(AWeaponBase* NewWeapon)
{
	if (MainWeapon)
	{
		MainWeapon->OnUnequip();
		MainWeapon->Destroy();
	}

	MainWeapon = NewWeapon;
}

void UCombatComponent::SetCombatEnabled(bool NewEnabled)
{
	bIsCombatEnabled = NewEnabled;

	ACharacterBase* OwningChar = Cast<ACharacterBase>(GetOwner());
	UAnimInst* AnimInst = Cast<UAnimInst>(OwningChar->GetMesh()->GetAnimInstance());
	bool bIsUsingAnimInterface = false;
	bIsUsingAnimInterface = AnimInst->Implements<UAnimInstanceInterface>();

	if (bIsUsingAnimInterface)
	{
		IAnimInstanceInterface::Execute_UpdateCombatEnabled(AnimInst, bIsCombatEnabled);
	}
}

void UCombatComponent::ShouldSaveAttack(bool WasSaved)
{
	bWasAttackedSaved = WasSaved;
}

void UCombatComponent::ResetAttack()
{
	AttackCount = 0;
	bWasAttackedSaved = false;
}

