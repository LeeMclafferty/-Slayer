// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slayer/Enum/CharacterState.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateBegin, ECharacterState, CharState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEnd, ECharacterState, CharState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLAYER_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/* -- Getters --*/
	UFUNCTION(BlueprintPure)
	ECharacterState GetCurrentState() const { return CurrentState; }
	
	/* -- Setters --*/
	void SetState(ECharacterState NewState);

	UPROPERTY()
	FOnStateBegin OnStateBegin;
	UPROPERTY()
	FOnStateEnd OnStateEnd;
	
	void ResetState();

	UFUNCTION(BlueprintPure)
	bool HasAnyState(TArray<ECharacterState> StatesToCheck);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called every frame
	ECharacterState CurrentState;

		
};
