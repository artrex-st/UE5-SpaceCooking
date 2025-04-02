// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SCMultiplayerSessionsSubsystem.h"

#include "SCUtilsLibrary.h"

USCMultiplayerSessionsSubsystem::USCMultiplayerSessionsSubsystem()
{
	USCUtilsLibrary::PrintStringScreen("@@ My MP session Subsystem is Constructed.");
}

void USCMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	USCUtilsLibrary::PrintStringScreen("@@ My MP session Subsystem was Initialized.");
}

void USCMultiplayerSessionsSubsystem::Deinitialize()
{
	USCUtilsLibrary::PrintStringScreen("@@ My MP session Subsystem was DeInitialized.");
	Super::Deinitialize();
}
