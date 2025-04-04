// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCDetectionActor.generated.h"

class USCTransporter;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDetectionActorOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDetectionActorOnDeactivated);

UCLASS()
class SPACECOOKING_API ASCDetectionActor : public AActor
{
	GENERATED_BODY()
	ASCDetectionActor();

public:
	UPROPERTY(BlueprintAssignable, Category = "SpaceCooking")
	FDetectionActorOnActivated OnActivated;
	UPROPERTY(BlueprintAssignable, Category = "SpaceCooking")
	FDetectionActorOnDeactivated OnDeactivated;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	USCTransporter* Transporter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	bool bActivated = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	FName TagAllowed = "Tag1";
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "SpaceCooking")
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	float DetectionTimerInterval = 0.5f;
	FTimerHandle DetectionTimer;
	UPROPERTY()
	TArray<AActor*> ActorsDetected;
	void TriggerDetectionEvent();
};
