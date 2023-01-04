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
	UFUNCTION(BlueprintPure, Category="Getter")
	class AWeaponBase* GetMainWeapon() { return MainWeapon; }
	UFUNCTION(BlueprintPure, Category = "Getter")
	bool IsCombatEnabled() { return bIsCombatEnabled; }

	/*-- Setters --*/
	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetMainWeapon(AWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetCombatEnabled(bool NewEnabled);
	
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
	class AWeaponBase* MainWeapon;
	bool bIsCombatEnabled;
};
