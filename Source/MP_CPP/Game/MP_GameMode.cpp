// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_GameMode.h"

#include "MP_GameState.h"

AMP_GameMode::AMP_GameMode()
{
	// 游戏是否应在第一位玩家登录时立即开始。此设置会影响 ReadyToStartMatch 的默认行为。
	bDelayedStart = true;
	MatchStartDelay = 5.f;
	bUseSeamlessTravel = true;	// 开启无缝切换地图属性
}

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
	
	GetWorldTimerManager().SetTimer(MatchStartTimer, this, &AMP_GameMode::StartDelayFinished, MatchStartDelay,false);
}

void AMP_GameMode::StartDelayFinished()
{
	StartMatch();
}
