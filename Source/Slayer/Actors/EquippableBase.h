// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquippableBase.generated.h"

UCLASS()
class SLAYER_API AEquippableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquippableBase();

	/* -- Getters --*/
	UFUNCTION(BlueprintPure, Category="Getters")
	class UPrimitiveComponent* GetItemMesh();

	UFUNCTION(BlueprintPure, Category="Getters")
	bool IsEquipped() { return bIsEquipped; }

	UFUNCTION(BlueprintPure, Category="Getters")
	FName GetUnequippedSocketName() { return UnequippedSocketName; }

	UFUNCTION(BlueprintPure, Category = "Getters")
	FName GetEquippedSocketName() { return EquippedSocketName; }

	/* --  Attachment --*/
	virtual void OnEquip();
	virtual void OnUnequip();
	void AttachActor(FName SocketName);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* -- Setters --*/
	UFUNCTION(BlueprintCallable, Category="Setters")
	void SetIsEquipped(bool bNew) { bIsEquipped = bNew; }


	/* --  Attachment --*/
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	FName UnequippedSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	FName EquippedSocketName;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* ItemStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* ItemSkeletalMesh;

	bool bIsEquipped;


};
