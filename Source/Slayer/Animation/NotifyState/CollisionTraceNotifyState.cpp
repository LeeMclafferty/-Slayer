// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Animation/NotifyState/CollisionTraceNotifyState.h"
#include "Slayer/Components/CollisionComponent.h"
#include "Slayer/Components/CombatComponent.h"
#include "Slayer/Actors/WeaponBase.h"

UCollisionTraceNotifyState::UCollisionTraceNotifyState()
{

}

void UCollisionTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UCombatComponent* CombatComp = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));

	if (!CombatComp)
		return;

	AWeaponBase* Weapon = CombatComp->GetMainWeapon();

	if (!Weapon)
		return;

	UCollisionComponent* CollisionComp = Weapon->GetCollisionComp();

	if (!CollisionComp)
		return;

	CollisionComp->EnableCollision();

}

void UCollisionTraceNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UCollisionTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCombatComponent* CombatComp = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
	if (!CombatComp)
		return;

	AWeaponBase* Weapon = CombatComp->GetMainWeapon();

	if (!Weapon)
		return;

	UCollisionComponent* CollisionComp = Weapon->GetCollisionComp();

	if (!CollisionComp)
		return;

	CollisionComp->DisableCollision();
}
