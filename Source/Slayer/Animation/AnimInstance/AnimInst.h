// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Slayer/Interfaces/AnimInstanceInterface.h"
#include "AnimInst.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UAnimInst : public UAnimInstance, public IAnimInstanceInterface
{
	GENERATED_BODY()

public:
	UAnimInst();

	UFUNCTION(BlueprintNativeEvent, Category = "AnimInterface")
	void UpdateCombatType(ECombatType NewCombatType);
	void UpdateCombatType_Implementation(ECombatType NewCombatType);

	UFUNCTION(BlueprintNativeEvent, Category = "AnimInterface")
	void UpdateWeaponAttachedToHand(bool NewAttachment);
	void UpdateWeaponAttachedToHand_Implementation(bool NewAttachment);

protected:
	UPROPERTY(BlueprintReadOnly)
	ECombatType CurrentCombatType;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponAttachedToHand;
};
