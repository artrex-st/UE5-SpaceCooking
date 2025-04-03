// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SCMultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
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
			SessionInterface.Pin()->OnCreateSessionCompleteDelegates.AddUObject(this, &USCMultiplayerSessionsSubsystem::OnCreateSessionCompleted);
		}
	}
}

void USCMultiplayerSessionsSubsystem::Deinitialize()
{
	//Super::Deinitialize();
}

void USCMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	//SessionInterface.
	if (ServerName.IsEmpty()) UE_LOG(LogTemp, Warning, TEXT("Server Name can't be Empty!"));

	FName SessionName = FName("Co-op Session");
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	bool IsLan = Online::GetSubsystem(GetWorld())->GetSubsystemName().IsEqual(FName("NULL"));
	SessionSettings.bIsLANMatch = IsLan;

	SessionInterface.Pin()->CreateSession(0, SessionName, SessionSettings);
}

void USCMultiplayerSessionsSubsystem::JoinServer(FString ServerName)
{
	if (ServerName.IsEmpty()) UE_LOG(LogTemp, Warning, TEXT("Server Name can't be Empty!"));
	USCUtilsLibrary::PrintStringScreen("Join Server: "+ ServerName);
}

void USCMultiplayerSessionsSubsystem::OnCreateSessionCompleted(FName SessionName, bool bIsSuccessful)
{
	FString Result = bIsSuccessful ? "Successful" : "Fail";
	USCUtilsLibrary::PrintStringScreen("Session " + SessionName.ToString() + " Created with:" + Result);

	if (bIsSuccessful)
	{
		GetWorld()->ServerTravel("/Game/FirstPerson/Maps/FirstPersonMap?listen");
	}
}