// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLAYER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	/*-- Getters --*/
	UFUNCTION(BlueprintPure, Category = "Getter")
	class AWeaponBase* GetMainWeapon() { return MainWeapon; }
	UFUNCTION(BlueprintPure, Category = "Getter")
	bool IsCombatEnabled() { return bIsCombatEnabled; }
	UFUNCTION(BlueprintPure, Category="Getter")
	bool WasAttackSaved() { return bWasAttackedSaved; } 

	/*-- Setters --*/
	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetMainWeapon(AWeaponBase* NewWeapon);
	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetCombatEnabled(bool NewEnabled);
	UFUNCTION(BlueprintCallable, Category = "Setter")
	void ShouldSaveAttack(bool WasSaved);
	
	
	UPROPERTY(BlueprintReadWrite, Category="Attack")
	int32 AttackCount;

	void ResetAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	class AWeaponBase* MainWeapon;
	bool bIsCombatEnabled;
	bool bWasAttackedSaved;
		
};
