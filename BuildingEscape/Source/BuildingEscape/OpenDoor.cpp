// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	this->DoorOpener = GetWorld()->GetFirstPlayerController()->GetPawn();
}


void UOpenDoor::OpenDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}


void UOpenDoor::CloseDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (this->PressurePlate->IsOverlappingActor(this->DoorOpener))
	{
		this->OpenDoor();
		this->DoorOpenLastTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - this->DoorOpenLastTime > this->DoorCloseDelay)
	{
		this->CloseDoor();
	}
}

