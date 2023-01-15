// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CollisionTraceNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SLAYER_API UCollisionTraceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UCollisionTraceNotifyState();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
};
