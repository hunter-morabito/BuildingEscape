// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"


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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();


}

void UOpenDoor::OpenDoor()
{
	// set the rotation
	Owner->SetActorRotation(FRotator(/*pitch(y)*/0.f, /*yaw (z)*/OpenAngle, /*roll(x)*/0.0f));
}

void UOpenDoor::CloseDoor()
{
	// set the rotation
	Owner->SetActorRotation(FRotator(0.f,-90.f,0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
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

