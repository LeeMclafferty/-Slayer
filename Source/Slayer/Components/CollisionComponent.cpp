// Fill out your copyright notice in the Description page of Project Settings.


#include "Slayer/Components/CollisionComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UCollisionComponent::UCollisionComponent()
	:bIsCollisionEnabled(false), CollisionMeshComponent(nullptr), 
	StartSocketName("collision_start"),EndSocketName("collision_end"), Radius(10.f)
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UCollisionComponent::ClearHitActors()
{
	AlreadyHitActors.Empty();
}

void UCollisionComponent::AddActorToIgnore(AActor* IgnoreActor)
{
	ActorsToIgnore.AddUnique(IgnoreActor);
}

void UCollisionComponent::RemoveActorToIgnore(AActor* IgnoreActor)
{
	ActorsToIgnore.Remove(IgnoreActor);
}

void UCollisionComponent::EnableCollision()
{
	ClearHitActors();
	bIsCollisionEnabled = true;
}

void UCollisionComponent::DisableCollision()
{
	bIsCollisionEnabled = false;
}

void UCollisionComponent::CollisionTrace()
{
	FVector StartLocation = CollisionMeshComponent->GetSocketLocation(StartSocketName);
	FVector EndLocation = CollisionMeshComponent->GetSocketLocation(EndSocketName);
	FQuat Rotation;

	FCollisionQueryParams Params;
	Params.AddIgnoredActors(ActorsToIgnore);
	TArray<FHitResult> OutHits;
	// Need to get capsule working.
	FCollisionShape ColShape = FCollisionShape::MakeCapsule(Radius, 50.f);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	UWorld* World = GetWorld();
	World->SweepMultiByObjectType(OutHits, StartLocation, EndLocation, Rotation, ObjectParams, ColShape, Params);
	
	// Just for the debug capsule
	//FRotator Rot(0,0,0);
	for (FHitResult Hit : OutHits)
	{
		LastHit = Hit;
		if (!AlreadyHitActors.Contains(Hit.GetActor()))
		{
			AlreadyHitActors.Add(Hit.GetActor());
			OnHit.Broadcast(LastHit);
			//UKismetSystemLibrary::DrawDebugCapsule(World, CollisionMeshComponent->GetSocketLocation("mid_point"), 50.f, Radius, Rot, FColor::Green, 2.0f, 2.0f);
		}
	}
	
	//UKismetSystemLibrary::DrawDebugCapsule(World, CollisionMeshComponent->GetSocketLocation("mid_point"), 50.f, Radius, Rot, FColor::Red, 2.0f, 2.0f);
}
