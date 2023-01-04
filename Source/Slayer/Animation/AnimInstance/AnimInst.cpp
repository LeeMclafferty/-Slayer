// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/AnimInstance/AnimInst.h"

UAnimInst::UAnimInst()
	:CurrentCombatType(ECombatType::ECT_None), bIsCombatEnabled(false)
{

}

void UAnimInst::UpdateCombatType_Implementation(ECombatType NewCombatType)
{
	CurrentCombatType = NewCombatType;
}

void UAnimInst::UpdateCombatEnabled_Implementation(bool NewAttachment)
{
	bIsCombatEnabled = NewAttachment;
}

