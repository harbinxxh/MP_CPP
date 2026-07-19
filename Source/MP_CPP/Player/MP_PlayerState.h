// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MP_PlayerState.generated.h"

// 声明动态多播委托：带一个参数
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupCountChanged, int32, Count);

UCLASS()
class MP_CPP_API AMP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMP_PlayerState();

	// 1、Override GetlifetimeReplicatedProps 此函数是注册需要复制变量的函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	int32 GetNumPickups() const { return NumPickups; };
	void SetNumPickups(int32 NewNumPickups);
	
	// 拾取数量变化的事件：拾取数量变动时，就触发这个事件
	FPickupCountChanged OnPickupCountChanged;
	
private:
	// 拾取数复制变量，并设置复制变量通知函数 OnRep_NumPickups()
	UPROPERTY(ReplicatedUsing = OnRep_NumPickups)
	int32 NumPickups;
	
	// 创建一个无返回值的复制通知函数
	UFUNCTION()
	void OnRep_NumPickups();
};
