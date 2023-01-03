// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Character/CharacterBase.h"
#include "EnhancedInputComponent.h"

#include "Slayer/Interfaces/InteractableInterface.h"
#include "Slayer/Actors/WeaponBase.h"

ACharacterBase::ACharacterBase()
	:MainWeapon(nullptr), HandSocket("weapon_r_soc"), ToggleCombatAction(nullptr), InteractAction(nullptr)
{
	HandSocket = "weapon_r_soc";
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
	}
}

void ACharacterBase::ToggleCombat()
{
	if (!MainWeapon) {
		return;
	}

	UAnimMontage* MontageToPlay;
	if (MainWeapon->IsAttachedToHand())
	{
		MontageToPlay = MainWeapon->GetWeaponSheathMontage();
		MainWeapon->SetAttachedToHand(false);
	}
	else
	{
		MontageToPlay = MainWeapon->GetWeaponDrawMontage();
		MainWeapon->SetAttachedToHand(true);
	}
	
	if(MontageToPlay)
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
}

void ACharacterBase::OnInteract()
{
	SweepForInteractable();
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
