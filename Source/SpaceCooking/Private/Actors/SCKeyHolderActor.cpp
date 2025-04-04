// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCKeyHolderActor.h"


ASCKeyHolderActor::ASCKeyHolderActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DetectionRoot"));
	SetRootComponent(RootComp);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Block);
	BaseMesh->SetIsReplicated(true);
	BaseMesh->SetupAttachment(RootComp);

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	KeyMesh->SetIsReplicated(true);
	KeyMesh->SetupAttachment(RootComp);
}

void ASCKeyHolderActor::BeginPlay()
{
	Super::BeginPlay();
	KeyMesh->SetVisibility(false);
}

void ASCKeyHolderActor::SetNewKey()
{
	KeyMesh->SetVisibility(true);
	GetWorldTimerManager().SetTimer(KeyRotationEffect, this, &ASCKeyHolderActor::PerformRotation, 0.016f, true, 0);
}

void ASCKeyHolderActor::PerformRotation()
{
	KeyMesh->AddRelativeRotation(FRotator(0, KeyMeshRotationSpeed * GetWorld()->GetDeltaSeconds(), 0));
}
