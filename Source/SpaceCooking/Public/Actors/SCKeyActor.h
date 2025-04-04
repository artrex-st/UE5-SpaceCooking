// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCKeyActor.generated.h"

class USphereComponent;

UCLASS()
class SPACECOOKING_API ASCKeyActor : public AActor
{
	GENERATED_BODY()
	ASCKeyActor();
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking",
		meta = (ToolTip = "Shape visual"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(ReplicatedUsing= OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere)
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

};
