// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
	if (PressurePlate == nullptr) { UE_LOG(LogTemp, Warning, TEXT("No PressurePlate")); }
}

void UOpenDoor::OpenDoor()
{
	//if (Owner == nullptr) { return; }
	OnOpenRequest.Broadcast();
	// set the rotation
	//Owner->SetActorRotation(FRotator(/*pitch(y)*/0.f, /*yaw (z)*/OpenAngle, /*roll(x)*/0.0f));
}

void UOpenDoor::CloseDoor()
{
	if (Owner == nullptr) { return; }
	// set the rotation
	Owner->SetActorRotation(FRotator(0.f,-90.f,0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > 50.f) // TODO make into parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	// check if time is up.
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) >= DoorCloseDelay) 
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;	// define a TArray that will hold an Array of Actor pointers
	if (PressurePlate == nullptr) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); // Initialize the TArray using this method

	// Iterate through them adding their masses
	for (const/*actor does not change*/ auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
