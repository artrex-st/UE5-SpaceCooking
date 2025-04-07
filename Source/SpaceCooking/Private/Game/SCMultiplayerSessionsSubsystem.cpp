// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SCMultiplayerSessionsSubsystem.h"

#include "SCUtilsLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

#include "Online/OnlineSessionNames.h"

USCMultiplayerSessionsSubsystem::USCMultiplayerSessionsSubsystem()
{
	//USCUtilsLibrary::PrintStringScreen("@@ My MP session Subsystem is Constructed.");
}

bool USCMultiplayerSessionsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}

	return false;
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
			SessionInterface.Pin()->OnDestroySessionCompleteDelegates.AddUObject(this, &USCMultiplayerSessionsSubsystem::OnDestroySessionCompleted);
			SessionInterface.Pin()->OnFindSessionsCompleteDelegates.AddUObject(this, &USCMultiplayerSessionsSubsystem::OnFindSessionsCompleted);
			SessionInterface.Pin()->OnJoinSessionCompleteDelegates.AddUObject(this, &USCMultiplayerSessionsSubsystem::OnJoinSessionsCompleted);
		}
	}
}

void USCMultiplayerSessionsSubsystem::Deinitialize()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface.Pin()->DestroySession(MySessionName);
	}
	//Super::Deinitialize();
}

void USCMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Name can't be Empty!"));
		OnMultiplayerSessionResponse.Broadcast(false);
		return;
	}

	MySessionName = FName("Co-op Session");

	if (FNamedOnlineSession* ExistingSession = SessionInterface.Pin()->GetNamedSession(MySessionName))
	{
		SessionInterface.Pin()->DestroySession(MySessionName);
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	const bool IsLan = Online::GetSubsystem(GetWorld())->GetSubsystemName().IsEqual(FName("NULL"));
	SessionSettings.bIsLANMatch = false;
	SessionSettings.Set(ServerNameFlag, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface.Pin()->CreateSession(0, MySessionName, SessionSettings);
}

void USCMultiplayerSessionsSubsystem::JoinServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Name can't be Empty!"));
		OnMultiplayerSessionResponse.Broadcast(false);
		return;
	}

	SessionSearchSettings = MakeShareable(new FOnlineSessionSearch());
	const bool IsLan = Online::GetSubsystem(GetWorld())->GetSubsystemName().IsEqual(FName("NULL"));
	SessionSearchSettings->bIsLanQuery = false;
	SessionSearchSettings->MaxSearchResults = 99999;
	SessionSearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	SessionInterface.Pin()->FindSessions(0, SessionSearchSettings.ToSharedRef());
}

void USCMultiplayerSessionsSubsystem::OnCreateSessionCompleted(FName SessionName, bool bIsSuccessful)
{
	const FString Result = bIsSuccessful ? "Successful" : "Fail";
	USCUtilsLibrary::PrintStringScreen("Session " + SessionName.ToString() + " Created with:" + Result);

	OnMultiplayerSessionResponse.Broadcast(bIsSuccessful);

	if (bIsSuccessful)
	{
		FString Path = GameMapPath.IsEmpty() ? "/Game/Levels/Gameplay_1?listen" : GameMapPath + "?listen";

		GetWorld()->ServerTravel(Path);
	}
}

void USCMultiplayerSessionsSubsystem::OnDestroySessionCompleted(FName SessionName, bool bIsSuccessful)
{
	FString Result = bIsSuccessful ? "Successful" : "Fail";
	USCUtilsLibrary::PrintStringScreen("Session " + SessionName.ToString() + " Destroied with:" + Result);

	if (bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void USCMultiplayerSessionsSubsystem::OnFindSessionsCompleted(bool bIsSuccessful)
{
	if (!bIsSuccessful || ServerNameToFind.IsEmpty())
	{
		OnMultiplayerSessionResponse.Broadcast(bIsSuccessful);
		return;
	}

	TArray<FOnlineSessionSearchResult> SessionResults = SessionSearchSettings->SearchResults;
	FOnlineSessionSearchResult* CorrectSession = nullptr;

	if (SessionResults.Num() > 0)
	{
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("%d sessions found."), SessionResults.Num()));
		for (FOnlineSessionSearchResult SessionResult : SessionResults)
		{
			if (SessionResult.IsValid())
			{
				FString Servername = "No-name";
				SessionResult.Session.SessionSettings.Get(ServerNameFlag, Servername);

				if (Servername.Equals(ServerNameToFind))
				{
					USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Found server name: %ls"), *Servername));
					CorrectSession = &SessionResult;
					break;
				}
			}
		}

		if (CorrectSession)
		{
			SessionInterface.Pin()->JoinSession(0, MySessionName, *CorrectSession);
			USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Joining on the server: %ls."), *ServerNameToFind));
			return;
		}

		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("No Sessions with the name: %ls Found."), *ServerNameToFind));
		ServerNameToFind = "";
		return;
	}

	OnMultiplayerSessionResponse.Broadcast(false);
	USCUtilsLibrary::PrintStringScreen("No Sessions Found.");
}

void USCMultiplayerSessionsSubsystem::OnJoinSessionsCompleted(FName SessionName,
	EOnJoinSessionCompleteResult::Type Result)
{
	if (EOnJoinSessionCompleteResult::Success == Result)
	{
		FString Address = "";

		if (const bool bSuccess = SessionInterface.Pin()->GetResolvedConnectString(SessionName, Address))
		{
			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
				
			USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Successful to join on the server: %s."), *SessionName.ToString()));
			return;
		}
	}

	switch (Result)
	{
	case EOnJoinSessionCompleteResult::SessionIsFull:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. The server %s is FULL."), *SessionName.ToString()));

		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. The server %s not exist any more."), *SessionName.ToString()));

		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. Could not retrieve the address of the server %s"), *SessionName.ToString()));

		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. You already on server %s"), *SessionName.ToString()));

		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. hmmm i don't know :(")));

		break;
	default:
		USCUtilsLibrary::PrintStringScreen(FString::Printf(TEXT("Error to join on. hmmm i don't know :(")));
	}

	OnMultiplayerSessionResponse.Broadcast(false);
}
