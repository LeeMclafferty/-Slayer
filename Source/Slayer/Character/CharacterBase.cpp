// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Character/CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"

#include "Slayer/Interfaces/InteractableInterface.h"
#include "Slayer/Actors/WeaponBase.h"
#include "Slayer/Interfaces/AnimInstanceInterface.h"
#include "Slayer/Components/CombatComponent.h"
#include "Slayer/Components/CollisionComponent.h"
#include "Slayer/Components/StateManagerComponent.h"


ACharacterBase::ACharacterBase()
	:Health(100.f), ToggleCombatAction(nullptr), InteractAction(nullptr)
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	StateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("State Manager"));

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakePointDamage.AddDynamic(this, &ACharacterBase::TakePointDamage);
	StateManager->OnStateBegin.AddDynamic(this, &ACharacterBase::StateBegin);
	StateManager->OnStateEnd.AddDynamic(this, &ACharacterBase::StateEnd);

}

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Completed, this, &ACharacterBase::ToggleCombat);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACharacterBase::OnInteract);
		// Perform Attach binding is in Event graph.
	}
}

bool ACharacterBase::CanToggleCombat()
{
	TArray<ECharacterState> StatesToCheck = {
		ECharacterState::ECS_Attacking, ECharacterState::ECS_Dead,
		ECharacterState::ECS_Disabled, ECharacterState::ECS_Dodging,
		ECharacterState::ECS_General };

	if (StateManager->HasAnyState(StatesToCheck) || GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	return true;
}

void ACharacterBase::ToggleCombat()
{

	if (!CanToggleCombat() || !CombatComponent->GetMainWeapon())
	{
		return;
	}
	
	StateManager->SetState(ECharacterState::ECS_General);

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

bool ACharacterBase::CanAttack()
{
	TArray<ECharacterState> StatesToCheck = {
		ECharacterState::ECS_Attacking, ECharacterState::ECS_Dead,
		ECharacterState::ECS_Disabled, ECharacterState::ECS_Dodging,
		ECharacterState::ECS_General };

	if (StateManager->HasAnyState(StatesToCheck) || GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	return true;
}

void ACharacterBase::Jump()
{
	// Need to add an anim notify that resets the jump later in the attack montages.
	if (CanJump())
	{
		Super::Jump();
	}
}

void ACharacterBase::Attack(int32 AttackIndex, bool UseRandomIndex)
{
	AWeaponBase* Weapon = CombatComponent->GetMainWeapon();

	if ( !CombatComponent|| !Weapon || !CombatComponent->IsCombatEnabled())
		return;

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

	StateManager->SetState(ECharacterState::ECS_Attacking);

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

	if (StateManager->GetCurrentState() == ECharacterState::ECS_Attacking)
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
		//DrawDebugSphere(GetWorld(), StartLocation, 100.f, 8, FColor::Green, false, 2.0f, 0, 2.0f);
	}
	else
	{
		//DrawDebugSphere(GetWorld(), StartLocation, 100.f, 8, FColor::Red, false, 2.0f, 0, 2.0f);
	}
}

void ACharacterBase::ContinueAttack_Implementation()
{
	StateManager->ResetState();

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
	StateManager->ResetState();
}

bool ACharacterBase::CanRecieveDamge_Implementation()
{
	if (StateManager->GetCurrentState() != ECharacterState::ECS_Dead)
	{
		return true;
	}

	return false;
}

void ACharacterBase::EnableRagDoll()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionProfileName("ragdoll");
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Pelvis"), true, true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("Pelvis"), 1.f);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCameraBoom()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Pelvis"));
	GetCameraBoom()->bDoCollisionTest = false;
	ApplyHitReactionVelocity(2000.f);

	AWeaponBase* Weapon = CombatComponent->GetMainWeapon();
	if (Weapon)
	{
		Weapon->SimulateWeaponPhysics();
	}
}

void ACharacterBase::Death()
{
	EnableRagDoll();

	// Add logic here for destroying dead actors after some time, probably will want some magic dissolve FX.
}

void ACharacterBase::ApplyHitReactionVelocity(float InitSpeed)
{
	// multiply -1 to go backwards.
	FVector NewVel = GetActorForwardVector() * (InitSpeed * -1);
	GetMesh()->SetPhysicsLinearVelocity(NewVel, false, FName("Pelvis"));
}

bool ACharacterBase::CanDodge()
{
	TArray<ECharacterState> StatesToCheck = {
	ECharacterState::ECS_Attacking, ECharacterState::ECS_Dead,
	ECharacterState::ECS_Disabled, ECharacterState::ECS_Dodging,
	ECharacterState::ECS_General };

	if (StateManager->HasAnyState(StatesToCheck) || GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	return true;
}

void ACharacterBase::TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	TakeDamgae(Damage);

	if(BloodVFX)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, HitLocation);
	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), (USoundBase*)HitSound, HitLocation, HitLocation.Rotation());
	if (HitMontage)
	{
		if (CanRecieveHitReations())
		{
			StateManager->SetState(ECharacterState::ECS_Disabled);
			GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
		}
			
	}


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

	StateManager->SetState(ECharacterState::ECS_Dodging);
	PerformDodge(MontageIndex, bUseRandom);
}

void ACharacterBase::TakeDamgae(float Damage)
{
	Health = FMath::Clamp(Health, 0, Health - Damage);

	if (Health <= 0)
	{
		StateManager->SetState(ECharacterState::ECS_Dead);
	}
}

bool ACharacterBase::CanRecieveHitReations() const
{
	TArray<ECharacterState> StatesToCheck = { ECharacterState::ECS_Dead };

	if (StateManager->HasAnyState(StatesToCheck))
	{
		return false;
	}

	return true;
}

bool ACharacterBase::CanJump() const
{
	TArray<ECharacterState> StatesToCheck = {
		ECharacterState::ECS_Attacking, ECharacterState::ECS_Dead,
		ECharacterState::ECS_Disabled, ECharacterState::ECS_Dodging,
		ECharacterState::ECS_General };

	if (StateManager->HasAnyState(StatesToCheck) || GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	return true;
}

void ACharacterBase::StateBegin(ECharacterState CharState)
{
	switch (CharState)
	{
	case ECharacterState::ECS_None:
		break;

	case ECharacterState::ECS_Attacking:
		break;

	case ECharacterState::ECS_Dodging:
		break;

	case ECharacterState::ECS_Dead:
		Death();
		break;

	case ECharacterState::ECS_Disabled:
		break;

	case ECharacterState::ECS_General:
		break;

	};
}

void ACharacterBase::StateEnd(ECharacterState CharState)
{
	switch (CharState)
	{
	case ECharacterState::ECS_None:
		break;

	case ECharacterState::ECS_Attacking:
		break;

	case ECharacterState::ECS_Dodging:
		break;

	case ECharacterState::ECS_Dead:
		break;

	case ECharacterState::ECS_Disabled:
		break;

	case ECharacterState::ECS_General:
		break;

	};
}