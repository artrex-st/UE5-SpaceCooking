// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCMovableActor.h"

#include "Actors/SCTransporter.h"
#include "Components/ArrowComponent.h"


ASCMovableActor::ASCMovableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	AActor::SetReplicateMovement(true);
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DetectionRoot"));
	SetRootComponent(RootComp);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetIsReplicated(true);
	Mesh->SetupAttachment(RootComp);

	StartPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("StartPoint"));
	StartPoint->SetArrowColor(FLinearColor::White);
	StartPoint->SetRelativeLocation(FVector::Zero());
	
	StartPoint->SetupAttachment(RootComp);

	EndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EndPoint"));
	EndPoint->SetArrowColor(FLinearColor::Yellow);
	EndPoint->SetRelativeLocation(FVector::UpVector * 300);

	EndPoint->SetupAttachment(RootComp);

	Transporter = CreateDefaultSubobject<USCTransporter>(TEXT("Transporter"));
}

void ASCMovableActor::BeginPlay()
{
	Super::BeginPlay();
	FVector StartPointLocation = GetActorLocation() + StartPoint->GetRelativeLocation();
	FVector EndPointLocation = GetActorLocation() + EndPoint->GetRelativeLocation();

	Transporter->SetTransporterPoints(StartPointLocation, EndPointLocation);
}


#if WITH_EDITOR
void ASCMovableActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		StartPoint->SetRelativeRotation((EndPoint->GetRelativeLocation() - StartPoint->GetRelativeLocation()).GetSafeNormal().Rotation());
		EndPoint->SetRelativeRotation((StartPoint->GetRelativeLocation() - EndPoint->GetRelativeLocation()).GetSafeNormal().Rotation());
	}
}

void ASCMovableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	StartPoint->SetRelativeRotation((EndPoint->GetRelativeLocation() - StartPoint->GetRelativeLocation()).GetSafeNormal().Rotation());
	EndPoint->SetRelativeRotation((StartPoint->GetRelativeLocation() - EndPoint->GetRelativeLocation()).GetSafeNormal().Rotation());
}
#endif
