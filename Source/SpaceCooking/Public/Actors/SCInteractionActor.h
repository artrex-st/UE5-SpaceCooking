// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCInteractionActor.generated.h"

class USCSwitcherComp;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionActorEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionActorDisable);

UCLASS()
class SPACECOOKING_API ASCInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	ASCInteractionActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FInteractionActorEnable OnActivated;
	FInteractionActorDisable OnDeactivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
	meta = (ToolTip = "Define o Tempo para mais de um Switcher ser ativado (se ouver)."))
	float SyncTimer = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay", meta = (ToolTip = ""))
	bool bSelfActorTrigger = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay")
	TArray<AActor*> OtherTriggers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "Define se é desativa com tempo ou nao."))
	bool bHasTimeToDeActive = false;

	UPROPERTY(ReplicatedUsing= OnRep_IsSwitcherEnabled, BlueprintReadWrite, VisibleAnywhere, Category = "SpaceCooking|OnlyVisible")
	bool bIsActive = false;

	UFUNCTION()
	void OnRep_IsSwitcherEnabled();
	
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void ActiveInteraction();
	void DeActiveInteraction();
	void PerformSwitcher();

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnDetectorActivated();
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnDetectorDeactivated();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible",
		meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "EnableMaterial"))
	UMaterialInterface* MaterialInstanceDisable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
		meta = (ToolTip = "EnableMaterial"))
	UMaterialInterface* MaterialInstanceEnable;

private:
	int ActivatedTriggerCount = 0;
	bool bAllTriggerActorsTriggered = false;
	float SwitcherTimerInterval = 0.016f;
	FTimerHandle SwitcherTimer;
};
