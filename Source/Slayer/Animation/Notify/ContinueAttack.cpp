// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/Notify/ContinueAttack.h"

#include "Slayer/Interfaces/CombatInterface.h"

UContinueAttack::UContinueAttack()
{

}

void UContinueAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	bool bUsesCombatInterface = false;

	if (MeshComp->GetOwner()->Implements<UCombatInterface>()) {
		bUsesCombatInterface = true;
	}

	if (bUsesCombatInterface)
	{
		ICombatInterface::Execute_ContinueAttack(MeshComp->GetOwner());
	}
}
