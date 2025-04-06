// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCKeyActor.generated.h"

class ASCKeyHolderActor;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKeyActorOnActivated);

UCLASS()
class SPACECOOKING_API ASCKeyActor : public AActor
{
	GENERATED_BODY()
	ASCKeyActor();
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SpaceCooking")
	FKeyActorOnActivated OnKeyActivated;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking|OnlyVisible",
		meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
	meta = (ToolTip = "Velocidade da animaçao da mesh"))
	float KeyMeshRotationSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceCooking|Gameplay",
	meta = (ToolTip = "Referencia do Suporte de Keys"))
	ASCKeyHolderActor* KeyHolderReference;

	UPROPERTY(ReplicatedUsing= OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere, Category = "SpaceCooking|OnlyVisible")
	bool bIsCollected = false;

	UFUNCTION()
	void OnRep_IsCollected();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	FTimerHandle KeyRotationEffect;
	void PerformRotation();
};
