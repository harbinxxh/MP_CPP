// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MP_GameState.generated.h"

/**
 * 
 */
UCLASS()
class MP_CPP_API AMP_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	// 1、Override GetlifetimeReplicatedProps 此函数是注册需要复制变量的函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void AddTeamMember(class APlayerController* PlayerController);
	bool IsTeamOne(class APlayerController* PlayerController) const;
	
private:
	// 保存玩家控制器数组
	// 用T对象指针，因为这是个成员变量指针
	UPROPERTY(Replicated)
	TArray<TObjectPtr<APlayerController>> TeamOne;
	
	UPROPERTY(Replicated)
	TArray<TObjectPtr<APlayerController>> TeamTwo;
};
