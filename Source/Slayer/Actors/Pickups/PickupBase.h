// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slayer/Interfaces/InteractableInterface.h"
#include "PickupBase.generated.h"

UCLASS()
class SLAYER_API APickupBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	APickupBase();
	
	/*-- Interface --*/
	UFUNCTION(BlueprintNativeEvent, Category="InteractableInterface")
	void Interact(AActor* Caller); 
	void Interact_Implementation(AActor* Caller);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UStaticMeshComponent* PickupMesh;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEquippableBase> ItemClass;
private:

	void SpawnPickupActor(AActor* Caller);


};
