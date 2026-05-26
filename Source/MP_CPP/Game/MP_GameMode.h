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
	virtual void StartMatch() override;
	// 玩家登录后，调用此函数
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	// 定时器句柄
	FTimerHandle MatchStartTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float MatchStartDelay;
	
	void StartDelayFinished();
};
