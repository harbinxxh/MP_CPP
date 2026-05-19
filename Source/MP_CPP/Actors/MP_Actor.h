// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MP_Actor.generated.h"

UCLASS()
class MP_CPP_API AMP_Actor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 客户端执行 RPC
	UFUNCTION(Client, Reliable)
	void Client_PrintActorName();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
