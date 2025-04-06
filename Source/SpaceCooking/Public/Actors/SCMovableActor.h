// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCMovableActor.generated.h"

class UArrowComponent;
class USCTransporter;

UCLASS()
class SPACECOOKING_API ASCMovableActor : public AActor
{
	GENERATED_BODY()
	ASCMovableActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible",
	meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	USCTransporter* Transporter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	UArrowComponent* StartPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	UArrowComponent* EndPoint;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif
	
};
