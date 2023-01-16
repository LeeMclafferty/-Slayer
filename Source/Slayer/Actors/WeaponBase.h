// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slayer/Actors/EquippableBase.h"
#include "Slayer/Enums/CombatTypes.h"
#include "WeaponBase.generated.h"

UCLASS()
class SLAYER_API AWeaponBase : public AEquippableBase
{
	GENERATED_BODY()

public:
	AWeaponBase();
	
	UPROPERTY(EditDefaultsOnly, Category="Initialization")
	ECombatType CombatType;
	
	/*-- Getters --*/
	UFUNCTION(BlueprintPure, Category="Getter")
	UAnimMontage* GetWeaponDrawMontage() { return WeaponDrawMontage; }
	UFUNCTION(BlueprintPure, Category = "Getter")
	UAnimMontage* GetWeaponSheathMontage() { return WeaponSheathMontage; }
	UFUNCTION(BlueprintPure, Category="Getter")
	TArray<UAnimMontage*> GetAttackMontages() { return AttackMontages; }
	UFUNCTION(BlueprintPure, Category = "Getter")
	class UCollisionComponent* GetCollisionComp() { return CollisionComponent; }

	/* --  Attachment --*/
	virtual void OnEquip() override;
	virtual void OnUnequip() override;

	UFUNCTION()
	void OnHit(FHitResult Hit);

	void SimulateWeaponPhysics();
protected:

	virtual void BeginPlay() override;


private:
	/*-- Animation --*/
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* WeaponDrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* WeaponSheathMontage;

	class UCombatComponent* CombatComponent;

	UPROPERTY(EditDefaultsOnly, Category="Animation|Attack")
	TArray<UAnimMontage*> AttackMontages;

	/* -- Collision --*/
	UPROPERTY(VisibleAnywhere)
	class UCollisionComponent* CollisionComponent;

	/* -- Stats --*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float WeaponDamage;
	UPROPERTY(EditDefaultsOnly, Category="Stats")
	TSubclassOf<class UDamageType> DamgeTypeClass;

};
