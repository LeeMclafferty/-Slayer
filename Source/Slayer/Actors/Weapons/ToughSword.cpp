// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Actors/Weapons/ToughSword.h"

AToughSword::AToughSword()
{
	CombatType = ECombatType::ECT_LightSword;
	UnequippedSocketName = "unequipped_hip_soc";
	EquippedSocketName = "weapon_light_soc";
}
