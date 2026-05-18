// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MP_Pickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class MP_CPP_API AMP_Pickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_Pickup();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SphereMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY(VisibleAnywhere)
	float HealthValue = 100.0f;
};
