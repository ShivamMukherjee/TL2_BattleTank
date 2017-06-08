// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrabber::Grab()
{
	/// Line trace and see if we reach actors with Physics Body set
	FHitResult Hit = this->GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
	AActor* ActorHit = Hit.GetActor();

	/// If an actor is reached, attach a Physics Handle
	if (ActorHit)
	{
		this->PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator::ZeroRotator
			//deprecated signature: true
		);
	}
}


void UGrabber::Release()
{
	this->PhysicsHandle->ReleaseComponent();
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	this->FindPhysicsHandleComponent();
	this->SetupInputComponent();
}

// Look for the input component (only appears at runtime)
void UGrabber::SetupInputComponent()
{
	this->InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!this->InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component attached to %s.\n\
			Go attach a Input Component to this."), *GetOwner()->GetName());
	}
	else
	{
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!this->PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component attached to %s.\n\
			Go attach a Physics Handle Component to this."), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
	if (this->PhysicsHandle->GrabbedComponent)
	{
		FVector Dummy, LineTraceEnd;
		GetReachLineEndpoints(OUT Dummy, OUT LineTraceEnd);

		// move object we're holding each frame
		this->PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Get the starting and ending positions of the line trace used for locating and grabbing a physics body
void UGrabber::GetReachLineEndpoints(FVector& PlayerViewPointLocation, FVector& OutLineTraceEnd)
{
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	OutLineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; 
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get the player's viewpoint
	FVector PlayerViewPointLocation;
	FVector LineTraceEnd;

	this->GetReachLineEndpoints(OUT PlayerViewPointLocation, OUT LineTraceEnd);

	// Draw a debug trace in the world to visualise
	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(30, 40, 50), false, 0.f, 0.f, 10.f);

	/// Line-trace (i.e. ray-cast) out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	return Hit;
}
