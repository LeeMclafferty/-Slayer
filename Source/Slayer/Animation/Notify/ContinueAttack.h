// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ContinueAttack.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UContinueAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UContinueAttack();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
