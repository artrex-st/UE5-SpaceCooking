// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCInteractionActor.h"

#include "SCUtilsLibrary.h"
#include "Actors/SCDetectionActor.h"
#include "Actors/SCKeyActor.h"
#include "Net/UnrealNetwork.h"


ASCInteractionActor::ASCInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionRoot"));
	SetRootComponent(RootComp);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);\
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
}

void ASCInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	UMaterialInterface* CurrentMaterial = bIsActive ? MaterialInstanceEnable : MaterialInstanceDisable;
	Mesh->SetMaterial(1, CurrentMaterial);

	if (bSelfActorTrigger) OtherTriggers.Add(this);

	for (AActor* Actor : OtherTriggers)
	{
		if (ASCDetectionActor* Detector = Cast<ASCDetectionActor>(Actor)) // TODO: Add Interface "Triggable || Activable"
		{
			Detector->OnActivated.AddDynamic(this, &ASCInteractionActor::OnDetectorActivated);
			Detector->OnDeactivated.AddDynamic(this, &ASCInteractionActor::OnDetectorDeactivated);
			continue;
		}
		
		if (ASCKeyActor* KeyActor = Cast<ASCKeyActor>(Actor))
		{
			KeyActor->OnKeyActivated.AddDynamic(this, &ASCInteractionActor::OnDetectorActivated);
			continue;
		}
	}
}

void ASCInteractionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASCInteractionActor, bIsActive);
}

void ASCInteractionActor::OnRep_IsSwitcherEnabled()
{
	UMaterialInterface* CurrentMaterial;

	if (bIsActive)
	{
		ActivatedTriggerCount++;
		OnActivated.Broadcast();
		CurrentMaterial = MaterialInstanceEnable;
		bAllTriggerActorsTriggered = OtherTriggers.Num() > 0 && ActivatedTriggerCount >= OtherTriggers.Num();

		if (!bHasLockOnEnd)
		{
			GetWorld()->GetTimerManager().SetTimer(SwitcherTimer, this,	&ASCInteractionActor::PerformSwitcher, SyncTimer, false);
		}
	}
	else
	{
		ActivatedTriggerCount--;
		OnDeactivated.Broadcast();
		CurrentMaterial = MaterialInstanceDisable;

		if (GetWorld()->GetTimerManager().IsTimerActive(SwitcherTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(SwitcherTimer);
		}
	}

	Mesh->SetMaterial(1, CurrentMaterial);
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Interaction: %d"), bIsActive));
}

void ASCInteractionActor::ActiveInteraction()
{
	if (!HasAuthority()) return;

	bIsActive = !bIsActive;
	OnRep_IsSwitcherEnabled();
}

void ASCInteractionActor::PerformSwitcher()
{
	bAllTriggerActorsTriggered = OtherTriggers.Num() > 0 && ActivatedTriggerCount >= OtherTriggers.Num();

	if (bAllTriggerActorsTriggered)
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("All Triggers OK")));
		return;
	}

	DeActiveInteraction();
}

void ASCInteractionActor::DeActiveInteraction()
{
	if (!bIsActive) return;
	ActiveInteraction();
}

void ASCInteractionActor::OnDetectorActivated()
{
	ActivatedTriggerCount++;
	bAllTriggerActorsTriggered = OtherTriggers.Num() > 0 && ActivatedTriggerCount >= OtherTriggers.Num();

	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Active Detectors: %d"), ActivatedTriggerCount));

	if (bAllTriggerActorsTriggered && HasAuthority())
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("All Triggers OK")));
	}
}

void ASCInteractionActor::OnDetectorDeactivated()
{
	ActivatedTriggerCount--;	
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("[Deactivate] Active Detectors: %d"), ActivatedTriggerCount));
}
