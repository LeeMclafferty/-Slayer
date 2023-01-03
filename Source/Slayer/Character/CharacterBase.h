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
	class AWeaponBase* GetMainWeapon() { return MainWeapon; }

	/*-- Setters --*/
	void SetMainWeapon(AWeaponBase* NewWeapon) { MainWeapon = NewWeapon; }
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*-- Input --*/
	UFUNCTION(Category=Input)
	void ToggleCombat();

	UFUNCTION(BlueprintCallable, Category = Input)
	void OnInteract();

	/*-- Combat --*/
	UPROPERTY(BlueprintReadOnly, Category="Combat|Weapon")
	class AWeaponBase* MainWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Attachment")
	FName HandSocket;



private:
	/*-- Input --*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleCombatAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	
	void SweepForInteractable();
};
