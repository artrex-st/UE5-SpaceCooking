// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SCMultiplayerSessionsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPACECOOKING_API USCMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	USCMultiplayerSessionsSubsystem();

public:
	TWeakPtr<IOnlineSession> SessionInterface;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void CreateServer(FString ServerName);
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void JoinServer(FString ServerName);

	UFUNCTION()
	void OnCreateSessionCompleted(FName SessionName, bool bIsSuccessful);
};
