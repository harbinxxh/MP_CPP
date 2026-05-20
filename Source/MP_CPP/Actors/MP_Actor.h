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
	
	// 服务器调用，客户端执行 RPC
	UFUNCTION(Client, Reliable)
	void Client_PrintActorName();
	
	// 客户端调用，服务器执行 RPC
	UFUNCTION(Server, Reliable)
	void Server_PrintActorName();
	
	// 当所有者更改时调用， 默认情况下不执行任何操作，但可以被重写
	virtual void OnRep_Owner() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
