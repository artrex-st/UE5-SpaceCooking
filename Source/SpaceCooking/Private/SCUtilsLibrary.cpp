// Fill out your copyright notice in the Description page of Project Settings.


#include "SCUtilsLibrary.h"

void USCUtilsLibrary::PrintStringScreen(const FString& String)
{
#if true
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
#endif
}

void USCUtilsLibrary::PrintStringScreen(const FString& String, const FColor& NewColor)
{
#if true
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, NewColor, String);
	}
#endif
}
