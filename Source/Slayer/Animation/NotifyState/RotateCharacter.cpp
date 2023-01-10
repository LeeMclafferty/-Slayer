// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/NotifyState/RotateCharacter.h"

#include "Slayer/Interfaces/CombatInterface.h"

URotateCharacter::URotateCharacter()
	:RotationSpeed(500.f)
{

}

void URotateCharacter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void URotateCharacter::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AActor* OwningActor = MeshComp->GetOwner();

	if (!OwningActor)
	{
		return;
	}

	bool bUsesInterface = OwningActor->Implements<UCombatInterface>();
	FRotator DesiredRot = FRotator::ZeroRotator;
	FRotator CurrentRot = OwningActor->GetActorRotation();

	if (bUsesInterface)
	{
		DesiredRot = ICombatInterface::Execute_GetDesiredRotation(OwningActor);
		OwningActor->SetActorRotation(FMath::RInterpTo(CurrentRot, DesiredRot, FrameDeltaTime, RotationSpeed));
	}

}

void URotateCharacter::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
