// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCMouseInteractiveComp.h"

#include "Actors/SCInteractionActor.h"
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

void USCMouseInteractiveComp::InteractRPC_Implementation()
{
	Interact();
}

//validaçao não necessaria
bool USCMouseInteractiveComp::InteractRPC_Validate()
{
	return true;
}

void USCMouseInteractiveComp::ReleaseRPC_Implementation()
{
	Release();
}

//validaçao não necessaria
bool USCMouseInteractiveComp::ReleaseRPC_Validate()
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

	DrawDebugLine(GetWorld(),Start, End, FColor::Emerald, false, 0.1f, 0, 1.f);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, Params))
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25, 8,FColor::Purple, false, 10);
		if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(Interactable))
		{
			GrabbedActor = HitResult.GetActor();
			GrabbedActor->DisableComponentsSimulatePhysics();
			GrabbedActor->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, InteractableSocket);
			//TODO: Add Feedback (VFX/SFX)
		}

		if (ASCInteractionActor* InteractionActor = Cast<ASCInteractionActor>(HitResult.GetActor()))
		{
			InteractionActor->ActiveInteraction();
			//TODO: Add Feedback (VFX/SFX)
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

