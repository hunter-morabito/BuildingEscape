// Fill out your copyright notice in the Description page of Project Settings.

#include "ChairPositionReport.h"
#include "Gameframework/Actor.h"


// Sets default values for this component's properties
UChairPositionReport::UChairPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
}


// Called when the game starts
void UChairPositionReport::BeginPlay()
{
	// Super constructor runs all code in parent class (BeginPlay())
	Super::BeginPlay();

	// get owner returns a pointer, hence '->'
	FString ObjectName = GetOwner()->GetName();
	// use this line to get location. ToString() output look like this:
	// "X=20.0, Y=40.0, Z=-1.0"
	// Switches from pointer to object
	FString j = GetOwner()->GetTransform().GetLocation().ToString();
	FString ObjectPos = "X=something, Y=something else, ";

	// need to dereference the pointer of the FString because the MACRO 'UE_LOG' only takes in TChar (character array) as an argument
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos);
	
}


// Called every frame
void UChairPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

