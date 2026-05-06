// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_MultiplayerUtils.h"

void UMP_MultiplayerUtils::PrintLocalNetRole(AActor* Actor)
{
	if (Actor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor [%s] Has Authority."), *Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor [%s] Does Not Have Authority."), *Actor->GetName());
	}
	
	switch (Actor->GetLocalRole())
	{
	case ROLE_Authority:
		// 服务器权威端：执行服务器逻辑，如状态验证、生成/销毁、AI决策等
		UE_LOG(LogTemp, Warning, TEXT("Actor Role: Authority - Server or single player"));
		break;

	case ROLE_AutonomousProxy:
		// 客户端自治代理：通常是玩家控制的 Pawn，执行本地预测和输入发送
		UE_LOG(LogTemp, Warning, TEXT("Actor Role: Autonomous Proxy - Network client, controlled locally"));
		break;

	case ROLE_SimulatedProxy:
		// 其他客户端的模拟代理：执行插值、平滑跟随等视觉表现逻辑
		UE_LOG(LogTemp, Warning, TEXT("Actor Role: Simulated Proxy - Network client receiving updates from server"));
		break;

	case ROLE_None:
		// 无网络角色（单机或未复制的对象）：通常等同于 Authority 或纯本地逻辑
		UE_LOG(LogTemp, Warning, TEXT("Actor Role: None - No networking"));
		break;

	default:
		// 不应出现的其他情况，可留空或记录警告
		UE_LOG(LogTemp, Warning, TEXT("Actor Role: Unknown role value"));
		break;
	}
}

void UMP_MultiplayerUtils::PrintRemoteNetRole(AActor* Actor)
{
	if (Actor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor [%s] Has Authority."), *Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor [%s] Does Not Have Authority."), *Actor->GetName());
	}
	
	switch (Actor->GetRemoteRole())
	{
	case ROLE_Authority:
		// 远程角色是权威端 → 通常出现在客户端上，表示该 Actor 的权威副本在服务器上
		// 可用于需要尊重服务器权威的逻辑，如禁用本地作弊行为
		UE_LOG(LogTemp, Warning, TEXT("Actor Remote Role: Authority - Server or single player"));
		break;

	case ROLE_AutonomousProxy:
		// 远程角色是自治代理 → 通常服务器看到某个客户端拥有的 Pawn 时，会显示此角色
		// 可用于服务器端处理来自该客户端的输入或属性同步
		UE_LOG(LogTemp, Warning, TEXT("Actor Remote Role: Autonomous Proxy - Network client, controlled locally"));
		break;

	case ROLE_SimulatedProxy:
		// 远程角色是模拟代理 → 服务器或其他客户端看到的、非本地控制的模拟 Actor
		// 可用于调整插值、LOD 或网络降级策略
		UE_LOG(LogTemp, Warning, TEXT("Actor Remote Role: Simulated Proxy - Network client receiving updates from server"));
		break;

	case ROLE_None:
		// 无远程角色（单机或未复制）
		UE_LOG(LogTemp, Warning, TEXT("Actor Remote Role: None - No networking"));
		break;

	default:
		// 不应出现的其他情况，可留空或记录警告
		UE_LOG(LogTemp, Warning, TEXT("Actor Remote Role: Unknown role value"));
		break;
	}
}
