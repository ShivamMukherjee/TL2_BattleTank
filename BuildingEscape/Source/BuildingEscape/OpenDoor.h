// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Angle at which door opens
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 90.f;

	// The pressure plate trigger volume thst affects the door's opening
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	// Delay for which door is closed
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;

	// Last time frame at which door was open
	float DoorOpenLastTime;

	// Mass that triggers the opening of the door
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	// Returns total mass on plate in kilograms
	float GetTotalMassOnPlate();
};
