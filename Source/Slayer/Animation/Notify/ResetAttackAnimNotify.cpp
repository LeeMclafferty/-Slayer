// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/Notify/ResetAttackAnimNotify.h"
#include "Slayer/Interfaces/CombatInterface.h"

UResetAttackAnimNotify::UResetAttackAnimNotify()
{

}

void UResetAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bool bUsesInterface = false;
	bUsesInterface = MeshComp->GetOwner()->Implements<UCombatInterface>();
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("Notify")));

	if (bUsesInterface)
	{
		ICombatInterface::Execute_ResetAttack(MeshComp->GetOwner());
	}
}
