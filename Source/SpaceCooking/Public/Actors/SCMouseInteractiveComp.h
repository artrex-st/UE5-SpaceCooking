// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCMouseInteractiveComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACECOOKING_API USCMouseInteractiveComp : public UActorComponent
{
	GENERATED_BODY()
	USCMouseInteractiveComp();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	float MaxGrabDistance = 400;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	float GrabRadius = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	float HoldDistance = 200;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	FName Interactable = FName("Interactable");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpaceCooking")
	FName InteractableSocket = FName("InteractableSocket");

	UFUNCTION(Server, Reliable, WithValidation)
	void InteractRPC();

	UFUNCTION(Server, Reliable, WithValidation)
	void ReleaseRPC();

	void Interact();
	void Release();

protected:
	virtual void BeginPlay() override;

private:
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpaceCooking")
	// UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY()
	AActor* GrabbedActor;
};
