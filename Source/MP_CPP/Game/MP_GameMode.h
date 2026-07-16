// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MP_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MP_CPP_API AMP_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMP_GameMode();
	
	// 从“等待启动”状态转变为“进行中”状态。
	// 你可以手动调用此方法，当 ReadyToStartMatch() 函数返回 true 时,系统也会自动调用此操作。
	virtual void StartMatch() override;
	// 玩家登录后，调用此函数
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	// 定时器句柄
	FTimerHandle MatchStartTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float MatchStartDelay; // 设置比赛延迟时间
	
	// 定时器回调函数
	void StartDelayFinished();
};
