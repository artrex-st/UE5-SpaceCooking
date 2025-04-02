// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SCMultiplayerSessionsSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "SCUtilsLibrary.h"

USCMultiplayerSessionsSubsystem::USCMultiplayerSessionsSubsystem()
{
	//USCUtilsLibrary::PrintStringScreen("@@ My MP session Subsystem is Constructed.");
}

void USCMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Super::Initialize(Collection);
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			USCUtilsLibrary::PrintStringScreen("Session is: ["+OnlineSubsystem->GetSubsystemName().ToString()+"] and Is valid");
		}
	}
}

void USCMultiplayerSessionsSubsystem::Deinitialize()
{
	//Super::Deinitialize();
}
