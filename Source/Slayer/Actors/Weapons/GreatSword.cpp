// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/Weapons/GreatSword.h"

AGreatSword::AGreatSword()
{
	CombatType = ECombatType::ECT_GreatSword;
	UnequippedSocketName = "unequipped_back_soc";
	EquippedSocketName = "weapon_great_soc";
}
