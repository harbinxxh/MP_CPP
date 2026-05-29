// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_PlayerState.h"
#include "Net/UnrealNetwork.h"

AMP_PlayerState::AMP_PlayerState()
{
	// 设置 PlayerState 更新频率
	SetNetUpdateFrequency(10.f);
}

void AMP_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 3、Call DOREPLIFETIME
	DOREPLIFETIME(ThisClass, NumPickups);
}

void AMP_PlayerState::SetNumPickups(int32 NewNumPickups)
{
	NumPickups = NewNumPickups;
	OnPickupCountChanged.Broadcast(NumPickups);
}

void AMP_PlayerState::OnRep_NumPickups()
{
	OnPickupCountChanged.Broadcast(NumPickups);
}
