// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SCSwitcherComp.h"

#include "SCUtilsLibrary.h"
#include "Actors/SCDetectionActor.h"
#include "Actors/SCInteractionActor.h"
#include "Actors/SCKeyActor.h"

USCSwitcherComp::USCSwitcherComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


void USCSwitcherComp::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = GetOwner();
	if (bSelfActorTrigger) OtherTriggers.Add(MyOwner);

	for (AActor* Actor : OtherTriggers)
	{
		if (ASCDetectionActor* Detector = Cast<ASCDetectionActor>(Actor)) // TODO: Add Interface "Triggable || Activable"
		{
			Detector->OnActivated.AddDynamic(this, &USCSwitcherComp::OnSwitcherActivated);
			Detector->OnDeactivated.AddDynamic(this, &USCSwitcherComp::OnSwitcherDeactivated);
			continue;
		}

		if (ASCKeyActor* KeyActor = Cast<ASCKeyActor>(Actor))
		{
			KeyActor->OnKeyActivated.AddDynamic(this, &USCSwitcherComp::OnSwitcherActivated);
			continue;
		}

		if (ASCInteractionActor* InteractionActor = Cast<ASCInteractionActor>(Actor))
		{
			InteractionActor->OnActivated.AddDynamic(this, &USCSwitcherComp::OnSwitcherActivated);
			InteractionActor->OnDeactivated.AddDynamic(this, &USCSwitcherComp::OnSwitcherDeactivated);
			continue;
		}
	}
}

void USCSwitcherComp::OnSwitcherActivated()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitcherTimer))
	{
		ActivatedTriggerCount--;
		GetWorld()->GetTimerManager().ClearTimer(SwitcherTimer);
	}

	ActivatedTriggerCount++;
	bAllTriggerActorsTriggered = OtherTriggers.Num() > 0 && ActivatedTriggerCount >= OtherTriggers.Num();
	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Active Switcher: %d"), ActivatedTriggerCount));
	GetWorld()->GetTimerManager().SetTimer(SwitcherTimer, this,	&USCSwitcherComp::PerformSwitcher, SyncTimer, true);
}

void USCSwitcherComp::OnSwitcherDeactivated()
{
	ActivatedTriggerCount--;

	if (GetWorld()->GetTimerManager().IsTimerActive(SwitcherTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(SwitcherTimer);
	}

	USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("DeActive Switcher: %d"), ActivatedTriggerCount));

	if (ASCInteractionActor* InteractionActor = Cast<ASCInteractionActor>(MyOwner))
	{
		InteractionActor->DeActiveInteraction();
	}
}

void USCSwitcherComp::PerformSwitcher()
{
	bAllTriggerActorsTriggered = OtherTriggers.Num() > 0 && ActivatedTriggerCount >= OtherTriggers.Num();

	if (MyOwner = GetOwner(); bAllTriggerActorsTriggered && MyOwner)
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("All Triggers OK")));
		return;
	}

	OnSwitcherDeactivated();
}
