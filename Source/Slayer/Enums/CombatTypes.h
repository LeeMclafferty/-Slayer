// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	ECT_None UMETA(DisplayName="None"),
	ECT_LightSword UMETA(DisplayName="Light Sword"),
	ECT_GreatSword UMETA(DisplayName="Great Sword")
};
