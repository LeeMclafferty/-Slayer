// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/Notify/ResetCombatAnimNotify.h"
#include "Slayer/Interfaces/CombatInterface.h"

UResetCombatAnimNotify::UResetCombatAnimNotify()
{

}

void UResetCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwningActor = MeshComp->GetOwner();

	bool bUsesComabtInterface = OwningActor->Implements<UCombatInterface>();
	if (bUsesComabtInterface)
	{
		ICombatInterface::Execute_ResetCombat(OwningActor);
	}
}
