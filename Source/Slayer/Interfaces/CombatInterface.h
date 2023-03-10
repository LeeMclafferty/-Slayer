// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLAYER_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category="CombatInterface")
	void ContinueAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ResetAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	FRotator GetDesiredRotation();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ResetCombat();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	bool CanRecieveDamge();
};
