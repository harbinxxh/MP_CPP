// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MP_HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MP_CPP_API UMP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMP_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	// 1、Override GetlifetimeReplicatedProps 此函数是注册需要复制变量的函数
	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetHealth(float NewHealth) { Health = NewHealth; };
	float GetHealth() const { return  Health; };
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	
	UFUNCTION()
	void OnRep_Health();
};
