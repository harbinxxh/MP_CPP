// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_HealthComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UMP_HealthComponent::UMP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// 在组件构建期间，建议使用“SetIsReplicatedByDefault”
	SetIsReplicatedByDefault(true);
	
	// 需要注意：构造函数中不能调用此函数
	// SetIsReplicated(true);
}


// Called when the game starts
void UMP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMP_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMP_HealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Health);
}

void UMP_HealthComponent::OnRep_Health()
{
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Green,
	FString::Printf(TEXT("Health: %f"), Health)
	);
}

