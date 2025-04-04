// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCMouseInteractiveComp.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "SpaceCooking/SpaceCookingCharacter.h"


// Sets default values for this component's properties
USCMouseInteractiveComp::USCMouseInteractiveComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	//PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComp"));
}

void USCMouseInteractiveComp::BeginPlay()
{
	Super::BeginPlay();
	
}

void USCMouseInteractiveComp::ServerInteract_Implementation()
{
	Interact();
}

bool USCMouseInteractiveComp::ServerInteract_Validate()
{
	return true;
}

void USCMouseInteractiveComp::ServerRelease_Implementation()
{
	Release();
}

bool USCMouseInteractiveComp::ServerRelease_Validate()
{
	return true;
}

void USCMouseInteractiveComp::Interact()
{
	if (GrabbedActor) Release();

	ASpaceCookingCharacter* OwnerCharacter = Cast<ASpaceCookingCharacter>(GetOwner());
	if (!OwnerCharacter)
		return;

	FVector Start = OwnerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector Forward = OwnerCharacter->GetControlRotation().Vector();
	FVector End = Start + Forward * MaxGrabDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	DrawDebugLine(GetWorld(),Start, End, FColor::Emerald, false, 0.5f, 0, 1.f);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, Params))
	{
		if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(Interactable))
		{
			GrabbedActor = HitResult.GetActor();
			GrabbedActor->DisableComponentsSimulatePhysics();
			GrabbedActor->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, InteractableSocket);
		}
	}
}

void USCMouseInteractiveComp::Release()
{
	if (GrabbedActor)
	{
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor = nullptr;
	}
}

