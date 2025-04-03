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
		USCUtilsLibrary::PrintStringScreen(Other->GetName()+" Begin Overlap!");

		if (Other->ActorHasTag(TagAllowed))
		{
			ActorsDetected.AddUnique(Other);
			bActivated = true;
			OnActivated.Broadcast();

			USCUtilsLibrary::PrintStringScreen(Other->GetName()+" Has The Key!");
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
		USCUtilsLibrary::PrintStringScreen(Other->GetName()+" End Overlap.");

		if (ActorsDetected.Contains(Other))
		{
			ActorsDetected.Remove(Other);
		}

		if (ActorsDetected.Num() < 0)
		{
			bActivated = false;
			OnDeactivated.Broadcast();
			GetWorld()->GetTimerManager().ClearTimer(DetectionTimer);
		}
	}
}

void ASCDetectionActor::TriggerDetectionEvent()
{
	if (ActorsDetected.IsEmpty())
	{
		USCUtilsLibrary::PrintStringScreen("No Actors Detected.");
		GetWorld()->GetTimerManager().ClearTimer(DetectionTimer);
		return;
	}

	FString Actors;
	for (AActor* Actor : ActorsDetected)
	{
		USCUtilsLibrary::PrintStringScreen(Actor->GetName()+" Has The Key!");
		Actors += Actor->GetName() + ", ";
	}

	USCUtilsLibrary::PrintStringScreen(Actors + " Still on Detection Overlap.");
}

