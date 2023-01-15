// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, HitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLAYER_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionComponent();

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetAlreadyHitActors() { return AlreadyHitActors; }
	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetActorsToIgnore() { return ActorsToIgnore; }
	UFUNCTION(BlueprintPure)
	FHitResult GetLastHit() { return LastHit; }
	UFUNCTION(BlueprintPure)
	bool IsCollisionEnabled() { return bIsCollisionEnabled; }
	UFUNCTION(BlueprintPure)
	UPrimitiveComponent* GetCollisionMeshComp() { return CollisionMeshComponent; }

	UFUNCTION(BlueprintCallable)
	void AddActorToIgnore(AActor* IgnoreActor);
	UFUNCTION(BlueprintCallable)
	void RemoveActorToIgnore(AActor* IgnoreActor);

	void SetCollisionMesh(UPrimitiveComponent* NewComp) { CollisionMeshComponent = NewComp; }

	void EnableCollision();
	void DisableCollision();
	void CollisionTrace();
	void ClearHitActors();


	UPROPERTY()
	FOnHit OnHit;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	bool bIsCollisionEnabled;
	TArray<AActor*> AlreadyHitActors;
	TArray<AActor*> ActorsToIgnore;
	UPrimitiveComponent* CollisionMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Initilization")
	FName StartSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Initilization")
	FName EndSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Initilization")
	float Radius;

	FHitResult LastHit;

		
};
