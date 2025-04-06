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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	FVector StartPoint = FVector::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	FVector EndPoint = FVector::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	bool bIsPointsSet = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay")
	float MoveTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "Define se deve travar quando chegar ao destino."))
	bool bHasLockOnEnd = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "Define se esse transporter se move quando ativado"))
	bool bSelfActorTrigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "Define se quais são os Actors que ativam esse Transporter."))
	TArray<AActor*> MyTriggerActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpaceCooking|OnlyVisible")
	int ActivatedTriggerCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
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
