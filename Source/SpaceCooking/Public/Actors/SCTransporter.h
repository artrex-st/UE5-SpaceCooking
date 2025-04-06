// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCTransporter.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACECOOKING_API USCTransporter : public UActorComponent
{
	GENERATED_BODY()
public:
	USCTransporter();
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	FVector StartPoint = FVector::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	FVector EndPoint = FVector::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	bool bIsPointsSet = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	float MoveTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Define se deve travar quando chegar ao destino."))
	bool bHasLockOnEnd = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Define se deve travar quando chegar ao destino."))
	bool bSelfActorTrigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TArray<AActor*> TriggerActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpaceCooking")
	int ActivatedTriggerCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	bool bAllTriggerActorsTriggered = false;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void SetTransporterPoints(FVector NewStartPoint, FVector NewEndPoint);
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnDetectionActivated();
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnDetectionDeactivated();
	void TransporterActivateMove();

private:
	float TransporterTimerInterval = 0.016f;
	FTimerHandle TransporterTimer;

	UPROPERTY()
	TObjectPtr<AActor> MyOwner;
};
