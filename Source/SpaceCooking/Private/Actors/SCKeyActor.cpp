// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCKeyActor.h"

#include "Actors/SCKeyHolderActor.h"
#include "Net/UnrealNetwork.h"


ASCKeyActor::ASCKeyActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DetectionRoot"));
	SetRootComponent(RootComp);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Mesh->SetIsReplicated(true);
	Mesh->SetupAttachment(RootComp);
}

void ASCKeyActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(KeyRotationEffect, this, &ASCKeyActor::PerformRotation, 0.016f, true, 0);
}

void ASCKeyActor::OnRep_IsCollected()
{
	Mesh->SetVisibility(false);
	SetHidden(true);
	KeyHolderReference->SetNewKey();
	GetWorldTimerManager().ClearTimer(KeyRotationEffect);
}

void ASCKeyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCKeyActor, bIsCollected);
}

void ASCKeyActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (!bIsCollected)
	{
		bIsCollected = true;
		OnRep_IsCollected();
	}
}

void ASCKeyActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ASCKeyActor::PerformRotation()
{
	Mesh->AddRelativeRotation(FRotator(0, KeyMeshRotationSpeed * GetWorld()->GetDeltaSeconds(), 0));
}

