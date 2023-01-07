// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slayer/SlayerCharacter.h"
#include "Slayer/Interfaces/CombatInterface.h"
#include "CharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API ACharacterBase : public ASlayerCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/*-- Getters --*/
	class UCombatComponent* GetCombatComponent() { return CombatComponent; }

	/*-- Setters --*/

	/*-- Combat Interface --*/
	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ContinueAttack();
	void ContinueAttack_Implementation();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*-- Input --*/
	UFUNCTION(Category="Input|Combat")
	void ToggleCombat();

	UFUNCTION(BlueprintCallable, Category = "Input|Interact")
	void OnInteract();

	//In Event graph to avoid complications with binding to functions that have parameters.
	UFUNCTION(BlueprintImplementableEvent, Category="Input|Combat")
	void PerformAttack(int32 AttackIndex, bool UseRandomIndex);

	/*-- Combat --*/
	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack(int32 AttackIndex, bool UseRandomIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttack(int32 AttackIndex, bool UseRandomIndex);
private:
	/*-- Input --*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleCombatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;

	void SweepForInteractable();
	
	/*-- Combat --*/
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* CombatComponent;
};
