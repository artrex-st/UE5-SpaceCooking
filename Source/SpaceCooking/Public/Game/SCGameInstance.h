// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACECOOKING_API USCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	void SpaceCookingRequestQuit();
};
