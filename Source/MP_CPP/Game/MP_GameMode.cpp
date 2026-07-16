// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_GameMode.h"

#include "MP_GameState.h"

AMP_GameMode::AMP_GameMode()
{
	// 游戏是否应在第一位玩家登录时立即开始。
	// 此设置会影响 ReadyToStartMatch 的默认行为。
	bDelayedStart = true;
	MatchStartDelay = 5.f;
	bUseSeamlessTravel = true;	// 开启无缝切换地图属性
}

// 从“等待启动”状态转变为“进行中”状态。
// 你可以手动调用此方法，当 ReadyToStartMatch() 函数返回 true 时,系统也会自动调用此操作。
void AMP_GameMode::StartMatch()
{
	Super::StartMatch();
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Orange,
	FString::Printf(TEXT("The match has started."))
	);
}

void AMP_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AMP_GameState* MP_GameState = GetGameState<AMP_GameState>();
	if (IsValid(MP_GameState))
	{
		MP_GameState->AddTeamMember(NewPlayer);
	}
}

void AMP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 创建定时器
	// 5s后，调用延迟函数 StartDelayFinished()
	GetWorldTimerManager().SetTimer(MatchStartTimer,
		this, &AMP_GameMode::StartDelayFinished, MatchStartDelay,false);
}

// 定时器回调函数
void AMP_GameMode::StartDelayFinished()
{
	// 启动游戏比赛
	StartMatch();
}
