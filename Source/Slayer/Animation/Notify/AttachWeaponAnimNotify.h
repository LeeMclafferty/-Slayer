// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachWeaponAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UAttachWeaponAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAttachWeaponAnimNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	bool bShouldAttachToHand;
};
