// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/Notify/ToggleCombatAnimNotify.h"

#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Components/CombatComponent.h"

UToggleCombatAnimNotify::UToggleCombatAnimNotify()
	:bShouldCombatEnabled(false)
{

}

void UToggleCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UCombatComponent* CombatComponent = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
	if (!CombatComponent) {
		//UE_LOG(LogTemp, Warning, TEXT("No Combat CompFound"));
		return;
	}

	if (!CombatComponent->GetMainWeapon()) {
		return;
	}

	AWeaponBase* Weapon = CombatComponent->GetMainWeapon();
	if (!Weapon) {
		return;
	}

	FName SocketToUse;
	if (bShouldCombatEnabled)
	{
		SocketToUse = Weapon->GetEquippedSocketName();
		CombatComponent->SetCombatEnabled(bShouldCombatEnabled);
	}
	else
	{
		SocketToUse = Weapon->GetUnequippedSocketName();
		CombatComponent->SetCombatEnabled(bShouldCombatEnabled);
	}

	Weapon->AttachActor(SocketToUse);
}
