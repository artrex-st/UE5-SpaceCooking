// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SCGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"

void USCGameInstance::Init()
{
	Super::Init();
}

void USCGameInstance::SpaceCookingRequestQuit()
{
	if (GetFirstLocalPlayerController())
	{
		//bRequestQuit = true;
		UKismetSystemLibrary::QuitGame(GetFirstLocalPlayerController(), GetFirstLocalPlayerController(), EQuitPreference::Quit, true);
	}
}
