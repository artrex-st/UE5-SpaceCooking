// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCTransporter.h"

#include "SCUtilsLibrary.h"
#include "Actors/SCDetectionActor.h"
#include "Actors/SCInteractionActor.h"
#include "Actors/SCKeyActor.h"
#include "Kismet/GameplayStatics.h"

USCTransporter::USCTransporter()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USCTransporter::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = GetOwner();

	if (bSelfActorTrigger) MyTriggerActors.Add(MyOwner);

	for (AActor* Actor : MyTriggerActors)
	{
		if (ASCDetectionActor* Detector = Cast<ASCDetectionActor>(Actor))
		{
			Detector->OnActivated.AddDynamic(this, &USCTransporter::OnDetectionActivated);
			Detector->OnDeactivated.AddDynamic(this, &USCTransporter::OnDetectionDeactivated);
			continue;
		}

		if (ASCKeyActor* KeyActor = Cast<ASCKeyActor>(Actor))
		{
			KeyActor->OnKeyActivated.AddDynamic(this, &USCTransporter::OnDetectionActivated);
			continue;
		}

		if (ASCInteractionActor* InteractionActor = Cast<ASCInteractionActor>(Actor))
		{
			InteractionActor->OnActivated.AddDynamic(this, &USCTransporter::OnDetectionActivated);
			InteractionActor->OnDeactivated.AddDynamic(this, &USCTransporter::OnDetectionDeactivated);
			continue;
		}
	}
}

void USCTransporter::SetTransporterPoints(FVector NewStartPoint, FVector NewEndPoint)
{
	if (NewStartPoint.Equals(NewEndPoint)) return;

	StartPoint = NewStartPoint;
	EndPoint = NewEndPoint;
	bIsPointsSet = true;
}

void USCTransporter::OnDetectionActivated()
{
	ActivatedTriggerCount++;
	bAllTriggerActorsTriggered = MyTriggerActors.Num() > 0 && ActivatedTriggerCount >= MyTriggerActors.Num();

	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Active Detectors: %d"), ActivatedTriggerCount));

	if (MyOwner = GetOwner(); bAllTriggerActorsTriggered && MyOwner && MyOwner->HasAuthority() && bIsPointsSet)
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("All Triggers OK")));
		bLockOnAll = bHasLockOnAll;

		if (GetWorld()->GetTimerManager().IsTimerActive(TransporterTimer)) return;

		GetWorld()->GetTimerManager().SetTimer(TransporterTimer, this,
				&USCTransporter::TransporterActivateMove, TransporterTimerInterval, true);
	}
}

void USCTransporter::OnDetectionDeactivated()
{
	if (bLockOnAll) return;

	ActivatedTriggerCount--;	
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("[Deactivate] Active Detectors: %d"), ActivatedTriggerCount));
}

void USCTransporter::TransporterActivateMove()
{
	if (!MyOwner) return;

	bAllTriggerActorsTriggered = MyTriggerActors.Num() > 0 && ActivatedTriggerCount >= MyTriggerActors.Num();
	FVector CurrentLocation = MyOwner->GetActorLocation();
	float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;
	FVector TargetLocation = bAllTriggerActorsTriggered ? EndPoint : StartPoint;

	if (!CurrentLocation.Equals(TargetLocation))
	{
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), Speed);
		MyOwner->SetActorLocation(NewLocation);
		return;
	}

	if (!bHasLockOnEnd) return;

	GetWorld()->GetTimerManager().ClearTimer(TransporterTimer);
}

