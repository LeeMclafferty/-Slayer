// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Components/StateManagerComponent.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateManagerComponent::SetState(ECharacterState NewState)
{
	if (NewState != CurrentState)
	{
		OnStateEnd.Broadcast(CurrentState);
		CurrentState = NewState;
		OnStateBegin.Broadcast(CurrentState);
	}
}

void UStateManagerComponent::ResetState()
{
	SetState(ECharacterState::ECS_None);
}

bool UStateManagerComponent::HasAnyState(TArray<ECharacterState> StatesToCheck)
{
	if (StatesToCheck.Contains(CurrentState))
	{
		return true;
	}

	return false;
}

