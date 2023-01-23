// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_None UMETA(DisplayName = "None"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_Dodging UMETA(DisplayName = "Dodging"),
	ECS_Dead UMETA(DisplayName = "Dead"),
	ECS_General UMETA(DisplayName = "General"),
	ECS_Disabled UMETA(DisplayName = "Disabled")
};

class SLAYER_API CharacterState
{
public:
	CharacterState();
	~CharacterState();
};
