// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Character/CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Slayer/Interfaces/InteractableInterface.h"
#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Interfaces/AnimInstanceInterface.h"
#include "Slayer/Components/CombatComponent.h"
#include "Slayer/Components/CollisionComponent.h"


ACharacterBase::ACharacterBase()
	:bIsTogglingCombat(false), bIsDodging(false), ToggleCombatAction(nullptr), InteractAction(nullptr)
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

bool ACharacterBase::CanToggleCombat()
{
	if (CombatComponent && CombatComponent->GetMainWeapon() && !bIsTogglingCombat && !CombatComponent->IsAttacking())
	{
		return true;
	}

	return false;
}

void ACharacterBase::ToggleCombat()
{

	if (!CanToggleCombat())
	{
		return;
	}
	
	bIsTogglingCombat = true;

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
	
	bIsTogglingCombat = false;
}

void ACharacterBase::OnInteract()
{
	SweepForInteractable();
}

bool ACharacterBase::CanAttack()
{
	if (CombatComponent && CombatComponent->GetMainWeapon() && !bIsTogglingCombat && !CombatComponent->IsAttacking() && !bIsDodging)
	{
		return true;
	}

	return false;
}

void ACharacterBase::Attack(int32 AttackIndex, bool UseRandomIndex)
{
	if ( !CombatComponent|| !CombatComponent->GetMainWeapon())
		return;

	AWeaponBase* Weapon = CombatComponent->GetMainWeapon();
	TArray<UAnimMontage*> AttackMontages = Weapon->GetAttackMontages();

	UAnimMontage* AttackToPlay = nullptr;
	if (UseRandomIndex)
	{
		AttackToPlay = AttackMontages[FMath::RandRange(0, AttackMontages.Num() - 1)];
	}
	else
	{
		if (AttackMontages[AttackIndex]) 
		{
			AttackToPlay = AttackMontages[AttackIndex];
		}
	}

	CombatComponent->SetIsAttacking(true);

	if (AttackToPlay && Weapon->IsEquipped())
		GetMesh()->GetAnimInstance()->Montage_Play(AttackToPlay);
	
	int32 LastIndex = AttackMontages.Num() - 1;

	if (CombatComponent->AttackCount + 1 > LastIndex)
	{
		CombatComponent->AttackCount = 0;
	}
	else
	{
		CombatComponent->AttackCount++;
	}
}

void ACharacterBase::OnAttack(int32 AttackIndex, bool UseRandomIndex)
{
	/* Calls the bp implementable event, that calls attack. This system is to be able to use input bindings 
	with parameters easier. I also use this to gate keep button spamming*/

	if (!CanAttack()) {
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
		OnAttack(CombatComponent->AttackCount, false);
	}
}

void ACharacterBase::ResetAttack_Implementation()
{
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->ResetAttack();
}

FRotator ACharacterBase::GetDesiredRotation_Implementation()
{
	FVector LastMove = GetCharacterMovement()->GetLastInputVector();

	if (!FMath::IsNearlyZero(LastMove.X ,.001))
	{
		return LastMove.ToOrientationRotator();
	}

	return GetActorRotation();
}

void ACharacterBase::ResetCombat_Implementation()
{
	CombatComponent->ResetAttack();
	bIsTogglingCombat = false;
	bIsDodging = false;
}

bool ACharacterBase::CanDodge()
{
	if (CombatComponent && !bIsTogglingCombat && !CombatComponent->IsAttacking() && !bIsDodging)
	{
		return true;
	}

	return false;
}

void ACharacterBase::Dodge(int32 MontageIndex, bool bUseRandom)
{

	UAnimMontage* MontageToPlay = nullptr;
	if (bUseRandom)
	{
		MontageToPlay = DodgeMontages[FMath::RandRange(0, DodgeMontages.Num() - 1)];
	}
	else
	{
		if (DodgeMontages[MontageIndex])
		{
			MontageToPlay = DodgeMontages[FMath::RandRange(0, DodgeMontages.Num() - 1)];
		}
	}

	if (MontageToPlay)
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
}

void ACharacterBase::OnDodge(int32 MontageIndex, bool bUseRandom)
{
	if (!CanDodge())
	{
		return;
	}

	bIsDodging = true;
	PerformDodge(MontageIndex, bUseRandom);
}
