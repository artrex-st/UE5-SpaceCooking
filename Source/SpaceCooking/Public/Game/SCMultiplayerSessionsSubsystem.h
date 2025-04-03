// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "SCMultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerResponseDelegate, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class SPACECOOKING_API USCMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	USCMultiplayerSessionsSubsystem();

public:
	UPROPERTY(BlueprintAssignable, Category = "SpaceCooking|Multiplayer")
	FServerResponseDelegate OnMultiplayerSessionResponse;
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearchSettings;
	bool bCreateServerAfterDestroy = false;
	FString ServerNameToFind = "";
	FString DestroyServerName = "";
	FName MySessionName = "";

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void CreateServer(FString ServerName);
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void JoinServer(FString ServerName);

	void OnCreateSessionCompleted(FName SessionName, bool bIsSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bIsSuccessful);
	void OnFindSessionsCompleted(bool bIsSuccessful);
	void OnJoinSessionsCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
