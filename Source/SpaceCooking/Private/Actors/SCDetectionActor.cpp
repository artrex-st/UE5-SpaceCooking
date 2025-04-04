// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCDetectionActor.h"

#include "SCUtilsLibrary.h"


ASCDetectionActor::ASCDetectionActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DetectionRoot"));
	SetRootComponent(RootComp);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ASCDetectionActor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

void ASCDetectionActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (Other->ActorHasTag(TagAllowed) && bActivated == false)
		{
			ActorsDetected.AddUnique(Other);
			bActivated = true;
			OnActivated.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(DetectionTimer, this,
				&ASCDetectionActor::TriggerDetectionEvent, DetectionTimerInterval, true);
		}
	}
}

void ASCDetectionActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		if (ActorsDetected.Contains(Other))
		{
			ActorsDetected.Remove(Other);
			OnDeactivated.Broadcast();
			USCUtilsLibrary::PrintStringScreen("Deactivate Broadcast");
		}

		if (ActorsDetected.Num() <= 0)
		{
			bActivated = false;
			USCUtilsLibrary::PrintStringScreen("Deactivate Bool");
			GetWorld()->GetTimerManager().ClearTimer(DetectionTimer);
		}
	}
}

void ASCDetectionActor::TriggerDetectionEvent()
{
	if (ActorsDetected.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(DetectionTimer);
		return;
	}
}

