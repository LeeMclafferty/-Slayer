// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetCombatAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UResetCombatAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UResetCombatAnimNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
