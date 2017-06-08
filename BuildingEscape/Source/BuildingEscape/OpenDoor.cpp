// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!this->PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate TriggerComponent uninitialised on %s"), *GetOwner()->GetName())
	}

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOnPlate() > this->TriggerMass)
	{
		this->OnOpen.Broadcast();
	}
	else
	{
		this->OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	
	if (!this->PressurePlate)
	{
		return TotalMass;
	}
	
	this->PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them while adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s overlapping!"), *Actor->GetName())
	}

	return TotalMass;
}

