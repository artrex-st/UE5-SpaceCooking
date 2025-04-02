// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SCUtilsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACECOOKING_API USCUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void PrintStringScreen(const FString& String);
};
