// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/Notify/AttachWeaponAnimNotify.h"

#include "Slayer/Character/CharacterBase.h"
#include "Slayer/Actors/WeaponBase.h"

UAttachWeaponAnimNotify::UAttachWeaponAnimNotify()
{

}

void UAttachWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (auto OwningChar = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (!OwningChar->GetMainWeapon()) {
			return;
		}

		AWeaponBase* Weapon = OwningChar->GetMainWeapon();

		FName SocketToUse;
		if (bShouldAttachToHand)
		{
			SocketToUse = Weapon->GetEquippedSocketName();
		}
		else
		{
			SocketToUse = Weapon->GetUnequippedSocketName();
		}

		Weapon->AttachActor(SocketToUse);
	}
}
