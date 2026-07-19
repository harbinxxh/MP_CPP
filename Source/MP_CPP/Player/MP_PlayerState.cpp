// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_PlayerState.h"
#include "Net/UnrealNetwork.h"

AMP_PlayerState::AMP_PlayerState()
{
	// 设置 PlayerState 玩家状态更新频率
	SetNetUpdateFrequency(10.f);
}

void AMP_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 3、Call DOREPLIFETIME
	DOREPLIFETIME(ThisClass, NumPickups);
}

// 设置拾取数数量
void AMP_PlayerState::SetNumPickups(int32 NewNumPickups)
{
	NumPickups = NewNumPickups;
	
	// 拾取数更新时，广播委托
	// 如果是服务器调用，可以将其广播出去
	OnPickupCountChanged.Broadcast(NumPickups);
}

// 当拾取数更新时，会触发此函数
void AMP_PlayerState::OnRep_NumPickups()
{
	// 拾取数更新时，广播委托
	OnPickupCountChanged.Broadcast(NumPickups);
}
