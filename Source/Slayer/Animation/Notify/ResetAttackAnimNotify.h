// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetAttackAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UResetAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UResetAttackAnimNotify();
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	
};
