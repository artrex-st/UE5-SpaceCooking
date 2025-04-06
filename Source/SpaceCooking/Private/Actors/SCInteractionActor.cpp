// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCInteractionActor.h"

#include "SCUtilsLibrary.h"
#include "Actors/SCSwitcherComp.h"
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

	SwitcherComp = CreateDefaultSubobject<USCSwitcherComp>(TEXT("SwitcherComponent"));
}

void ASCInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	UMaterialInterface* CurrentMaterial = bIsActive ? MaterialInstanceEnable : MaterialInstanceDisable;
	Mesh->SetMaterial(1, CurrentMaterial);
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
		OnActivated.Broadcast();
		CurrentMaterial = MaterialInstanceEnable;
	}
	else
	{
		OnDeactivated.Broadcast();
		CurrentMaterial = MaterialInstanceDisable;
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

void ASCInteractionActor::DeActiveInteraction()
{
	Mesh->SetMaterial(1, MaterialInstanceDisable);
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Interaction: %d"), bIsActive));
}

