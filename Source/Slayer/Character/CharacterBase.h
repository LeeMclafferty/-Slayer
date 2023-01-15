// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slayer/SlayerCharacter.h"
#include "Slayer/Interfaces/CombatInterface.h"
#include "Slayer/Components/CombatComponent.h"
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
	UFUNCTION(BlueprintPure, Category="Getter")
	class UCombatComponent* GetCombatComponent() { return CombatComponent; }

	/*-- Setters --*/

	/*-- Combat Interface --*/
	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ContinueAttack();
	void ContinueAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ResetAttack();
	void ResetAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	FRotator GetDesiredRotation();
	FRotator GetDesiredRotation_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "CombatInterface")
	void ResetCombat();
	void ResetCombat_Implementation();


	UPROPERTY(BlueprintReadWrite, Category="Combat")
	bool bIsTogglingCombat;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bIsDodging;
	
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

	UFUNCTION(BlueprintImplementableEvent, Category = "Input|Interact")
	void PerformDodge(int32 MontageIndex, bool bUseRandom);


	/*-- Combat --*/
	UFUNCTION(BlueprintCallable, Category="Combat")
	void Attack(int32 AttackIndex, bool UseRandomIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttack(int32 AttackIndex, bool UseRandomIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Dodge(int32 MontageIndex, bool bUseRandom);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnDodge(int32 MontageIndex, bool bUseRandom);

private:
	/*-- Input --*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleCombatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DodgeAction;


	void SweepForInteractable();
	

	/*-- Combat --*/
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* CombatComponent;

	bool CanToggleCombat();
	bool CanAttack();
	bool CanDodge();

	UPROPERTY(EditDefaultsOnly, Category = "Animation|Attack")
	TArray<UAnimMontage*> DodgeMontages;

};
