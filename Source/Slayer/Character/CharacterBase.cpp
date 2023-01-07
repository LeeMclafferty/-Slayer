// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Character/CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "Math/UnrealMathUtility.h"

#include "Slayer/Interfaces/InteractableInterface.h"
#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Interfaces/AnimInstanceInterface.h"
#include "Slayer/Components/CombatComponent.h"


ACharacterBase::ACharacterBase()
	:ToggleCombatAction(nullptr), InteractAction(nullptr)
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Completed, this, &ACharacterBase::ToggleCombat);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACharacterBase::OnInteract);
		// Perform Attach binding is in Event graph.
	}
}

void ACharacterBase::ToggleCombat()
{
	if (!CombatComponent->GetMainWeapon()) {
		return;
	}

	UAnimMontage* MontageToPlay;
	if (CombatComponent->IsCombatEnabled())
	{
		MontageToPlay = CombatComponent->GetMainWeapon()->GetWeaponSheathMontage();
	}
	else
	{
		MontageToPlay = CombatComponent->GetMainWeapon()->GetWeaponDrawMontage();
	}
	
	if(MontageToPlay)
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
}

void ACharacterBase::OnInteract()
{
	SweepForInteractable();
}

void ACharacterBase::Attack(int32 AttackIndex, bool UseRandomIndex)
{
	TArray<UAnimMontage*> AttackMontages = CombatComponent->GetMainWeapon()->GetAttackMontages();

	UAnimMontage* AttackToPlay = nullptr;
	if (UseRandomIndex)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("Rand")));
		AttackToPlay = AttackMontages[FMath::FRandRange(0, AttackMontages.Num())];
	}
	else
	{
		AttackToPlay = AttackMontages[AttackIndex];

	}

	CombatComponent->SetIsAttacking(true);

	if (AttackToPlay)
		GetMesh()->GetAnimInstance()->Montage_Play(AttackToPlay);
}

void ACharacterBase::OnAttack(int32 AttackIndex, bool UseRandomIndex)
{
	/* Calls the bp implementable event, that calls attack. This system is to be able to use input bindings 
	with parameters easier. I also use this to gate keep button spamming*/

	if (!CombatComponent) {
		return;
	}

	if (CombatComponent->IsAttacking())
	{
		CombatComponent->ShouldSaveAttack(true);
	}
	else
	{
		PerformAttack(AttackIndex, UseRandomIndex);
	}
}

void ACharacterBase::SweepForInteractable()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation();
	FRotator Rotation;

	FCollisionQueryParams Params;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	Params.AddIgnoredActors(ActorsToIgnore);

	FHitResult OutHit;
	FCollisionShape ColShape = FCollisionShape::MakeSphere(100.f);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	GetWorld()->SweepSingleByObjectType(OutHit, StartLocation, EndLocation, FQuat::MakeFromRotator(Rotation), ObjectParams,
		ColShape, Params);


	bool bUsesInterface = false;

	if (OutHit.GetActor())
	{
		bUsesInterface = OutHit.GetActor()->Implements<UInteractableInterface>();
	}

	if (bUsesInterface)
	{
		IInteractableInterface::Execute_Interact(OutHit.GetActor(), this);
		DrawDebugSphere(GetWorld(), StartLocation, 100.f, 8, FColor::Green, false, 2.0f, 0, 2.0f);
	}
	else
	{
		DrawDebugSphere(GetWorld(), StartLocation, 100.f, 8, FColor::Red, false, 2.0f, 0, 2.0f);
	}
}

void ACharacterBase::ContinueAttack_Implementation()
{
	CombatComponent->SetIsAttacking(false);

	if (CombatComponent->WasAttackSaved())
	{
		CombatComponent->ShouldSaveAttack(false);
		OnAttack(CombatComponent->GetAttackCount(), false);
	}
}
