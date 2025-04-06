// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCSwitcherComp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACECOOKING_API USCSwitcherComp : public UActorComponent
{
	GENERATED_BODY()

public:
	USCSwitcherComp();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	bool bIsPointsSet = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Define o Tempo para mais de um Switcher ser ativado (se ouver)."))
	float SyncTimer = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Define se é ou nao de uso unico."))
	bool bHasLockOnEnd = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking", meta = (ToolTip = ""))
	bool bSelfActorTrigger = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TArray<AActor*> OtherTriggers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpaceCooking")
	int ActivatedTriggerCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	bool bAllTriggerActorsTriggered = false;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnSwitcherActivated();
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnSwitcherDeactivated();
	void PerformSwitcher();

private:
	float SwitcherTimerInterval = 0.016f;
	FTimerHandle SwitcherTimer;

	UPROPERTY()
	TObjectPtr<AActor> MyOwner;
};
