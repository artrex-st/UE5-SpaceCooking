// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCKeyHolderActor.generated.h"

class ASCKeyActor;

UCLASS()
class SPACECOOKING_API ASCKeyHolderActor : public AActor
{
	GENERATED_BODY()
public:
	ASCKeyHolderActor();
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Shape visual da base"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Shape visual da Key"))
	UStaticMeshComponent* KeyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Velocidade de rotaçao da KeyMesh na base"))
	float KeyMeshRotationSpeed = 100;

	void SetNewKey();

private:
	FTimerHandle KeyRotationEffect;
	void PerformRotation();
};
