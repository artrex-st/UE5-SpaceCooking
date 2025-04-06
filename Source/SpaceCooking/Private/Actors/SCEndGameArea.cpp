// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Actors/SCEndGameArea.h"

#include "Components/BoxComponent.h"
#include "SpaceCooking/SpaceCookingCharacter.h"


// Sets default values
ASCEndGameArea::ASCEndGameArea()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	EndGameAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EndGameAreaBox"));
	SetRootComponent(EndGameAreaBox);

	WinCondition = false;
}

void ASCEndGameArea::BeginPlay()
{
	Super::BeginPlay();

	EndGameAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ASCEndGameArea::OnPlayerBeginOverlap);
	EndGameAreaBox->OnComponentEndOverlap.AddDynamic(this, &ASCEndGameArea::OnPlayerEndOverlap);
}

void ASCEndGameArea::CheckAllPlayers()
{
	if (HasAuthority())
	{
		if (!WinCondition)
		{
			TArray<AActor*> OverlapActors;
			EndGameAreaBox->GetOverlappingActors(OverlapActors, ASpaceCookingCharacter::StaticClass());
			WinCondition = (OverlapActors.Num() == 2);

			if (WinCondition) MulticastRPCWin();
		}
	}

}

void ASCEndGameArea::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorldTimerManager().IsTimerActive(EndGameAreaTimer)) return;

	GetWorldTimerManager().SetTimer(EndGameAreaTimer, this,
			&ASCEndGameArea::CheckAllPlayers, EndGameTimerInterval, true);
}

void ASCEndGameArea::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> OverlapActors;
	EndGameAreaBox->GetOverlappingActors(OverlapActors, ASpaceCookingCharacter::StaticClass());

	if (OverlapActors.Num() < 0 && GetWorldTimerManager().IsTimerActive(EndGameAreaTimer))
	{
		GetWorldTimerManager().ClearTimer(EndGameAreaTimer);
	}
}

void ASCEndGameArea::MulticastRPCWin_Implementation()
{
	OnWinCondition.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("@@ Win"));
}

