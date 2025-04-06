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

	UPROPERTY(ReplicatedUsing= OnRep_IsSwitcherEnabled, BlueprintReadWrite, VisibleAnywhere)
	bool bIsActive = false;

	UFUNCTION()
	void OnRep_IsSwitcherEnabled();
	
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void ActiveInteraction();
	void DeActiveInteraction();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "EnableMaterial"))
	UMaterialInterface* MaterialInstanceDisable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "EnableMaterial"))
	UMaterialInterface* MaterialInstanceEnable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	USCSwitcherComp* SwitcherComp;
};
