// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_Actor.h"


// Sets default values
AMP_Actor::AMP_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bNetLoadOnClient = true;
	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void AMP_Actor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Client_PrintActorName();
	}
}

void AMP_Actor::Client_PrintActorName_Implementation()
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += GetName();
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	30.f,
	FColor::Yellow,
	MessageString
	);
}

// Called every frame
void AMP_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

