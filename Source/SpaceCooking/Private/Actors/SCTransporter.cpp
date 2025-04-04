// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCTransporter.h"

#include "SCUtilsLibrary.h"
#include "Actors/SCDetectionActor.h"
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

	for (AActor* Actor : TriggerActors)
	{
		if (ASCDetectionActor* Detector = Cast<ASCDetectionActor>(Actor))
		{
			Detector->OnActivated.AddDynamic(this, &USCTransporter::OnDetectionActivated);
			Detector->OnDeactivated.AddDynamic(this, &USCTransporter::OnDetectionDeactivated);
		}
	}

	SetTransporterPoints(MyOwner->GetActorLocation(), MyOwner->GetActorLocation() + (FVector::UpVector * 200));
}

void USCTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	bAllTriggerActorsTriggered = TriggerActors.Num() > 0 && ActivatedTriggerCount >= TriggerActors.Num();

	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Active Detectors: %d"), ActivatedTriggerCount));

	if (MyOwner = GetOwner(); bAllTriggerActorsTriggered && MyOwner && MyOwner->HasAuthority() && bIsPointsSet)
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("All Triggers OK")));

		if (GetWorld()->GetTimerManager().IsTimerActive(TransporterTimer)) return;

		GetWorld()->GetTimerManager().SetTimer(TransporterTimer, this,
				&USCTransporter::TransporterActivateMove, TransporterTimerInterval, true);
	}
}

void USCTransporter::OnDetectionDeactivated()
{
	ActivatedTriggerCount--;	
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("[Deactivate] Active Detectors: %d"), ActivatedTriggerCount));
}

void USCTransporter::TransporterActivateMove()
{
	if (!MyOwner) return;

	bAllTriggerActorsTriggered = TriggerActors.Num() > 0 && ActivatedTriggerCount >= TriggerActors.Num();
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

