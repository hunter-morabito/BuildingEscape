// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{	
	// Boiler bool asking me if I want to tick every frame.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	// always leave
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Always put the class before the method name in C++
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
		UE_LOG(LogTemp, Error, TEXT("The Physics Handle dne for %s"), *(GetOwner()->GetName()));
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		// "Grab" is the name of the command on the Player Controller that I have bound to RMB
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The Input Handle dne for %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grab Pressed"));

	// LINE TRACE and try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		// If we hit something then attach a physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			/*the primitive component*/
			ComponentToGrab,
			NAME_None,
			/*The grab location, in this case the vertex of the object*/
			ComponentToGrab->GetOwner()->GetActorLocation(),
			/*Keep the original rotation of the actor*/
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
	
	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Grab Released"));

	// TODO release physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

	// If the physics handle is attached
		// move the object that we're holding 


	// See what we hit
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Draw a red line
	/*DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(255,0,0),
	false,
	0.f,
	0.f,
	10.f
	);*/

	/// Setup Query parameters
	FCollisionQueryParams TraceParameters(
		/*The Tag of the trace*/
		FName(TEXT("")),
		/*Is the collision simple physics*/
		false,
		/*Choose AActors to ignore*/
		GetOwner());

	/// Line-Trace (Ray-cast) out to reach distance
	// Define the object to be referenced as the object that was hit
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		/*Send in a reference to be initialized*/
		OUT Hit,
		/*Send in a pointer to the start of the ray*/
		GetReachLineStart(),
		/*Send in a pointer to the end of the ray*/
		GetReachLineEnd(),
		/*List of object types it's am looking for*/
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		/*Additional parameters used for the trace (see above what has been added*/
		TraceParameters
	);

	// check if an actor that has been queried for has been hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(ActorHit->GetName()));
	}

	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return (PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach));
}
