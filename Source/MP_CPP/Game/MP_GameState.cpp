// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_GameState.h"

#include "Net/UnrealNetwork.h"

void AMP_GameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 3、Call DOREPLIFETIME
	DOREPLIFETIME(ThisClass, TeamOne);
	DOREPLIFETIME(ThisClass, TeamTwo);
}

void AMP_GameState::AddTeamMember(class APlayerController* PlayerController)
{
	if (TeamOne.Num() > TeamTwo.Num())
	{
		TeamTwo.Add(PlayerController);
	}
	else
	{
		TeamOne.Add(PlayerController);
	}
}

bool AMP_GameState::IsTeamOne(class APlayerController* PlayerController) const
{
	return TeamOne.Contains(PlayerController);
}
