// Fill out your copyright notice in the Description page of Project Settings.


#include "SCUtilsLibrary.h"

void USCUtilsLibrary::PrintStringScreen(const FString& String)
{
#if UE_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
#endif
}
