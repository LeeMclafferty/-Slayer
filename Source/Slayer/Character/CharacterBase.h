// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slayer/SlayerCharacter.h"
#include "CharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API ACharacterBase : public ASlayerCharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/*-- Getters --*/

	/*-- Setters --*/
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*-- Input --*/
	UFUNCTION(Category=Input)
	void ToggleCombat();

	UFUNCTION(BlueprintCallable, Category = Input)
	void OnInteract();

private:
	/*-- Input --*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleCombatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	
	void SweepForInteractable();
	
	/*-- Combat --*/
	class UCombatComponent* CombatComponent;
};
