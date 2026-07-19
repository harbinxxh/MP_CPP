// Copyright Epic Games, Inc. All Rights Reserved.


#include "MP_CPPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "MP_CPP.h"
#include "Player/MP_PlayerState.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "UI/MP_PickupCountWidget.h"

void AMP_CPPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 若不是本地玩家控制器就返回
	if (!IsLocalController()) return;
	
	// 添加 拾取物 Widget 控件
	PickupCountWidget = CreateWidget<UMP_PickupCountWidget>(this, PickupCountWidgetClass);
	if (IsValid(PickupCountWidget))
	{
		PickupCountWidget->AddToViewport(0);
	}

	// 服务器端绑定委托代理函数
	// 为拾取数变更，绑定委托回调函数
	if (HasAuthority())
	{
		AMP_PlayerState* MP_PlayerState = GetPlayerState<AMP_PlayerState>();
		if (!IsValid(MP_PlayerState)) return;
	
		MP_PlayerState->OnPickupCountChanged.AddDynamic(this, &ThisClass::OnPickupCountChanged);
	}
	
	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogMP_CPP, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
}

void AMP_CPPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

// OnRep_PlayerState 函数主要在客户端自动触发
// 当服务器完成 PlayerState 的创建、更新或同步后，就在所有客户端上调用该函数，作为数据已经准备好的安全信号
void AMP_CPPPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// IsLocalController() 函数判断是否为本地玩家控制器
	// 若不是本地玩家控制器就返回
	if (!IsLocalController()) return;
	
	// 只有当是本地玩家控制器时，才会绑定回调。
	AMP_PlayerState* MP_PlayerState = GetPlayerState<AMP_PlayerState>();
	if (!IsValid(MP_PlayerState)) return;
	
	// 为拾取数变更，绑定委托回调函数
	MP_PlayerState->OnPickupCountChanged.AddDynamic(this, &ThisClass::OnPickupCountChanged);
}

// 当拾取数量变动时触发的回调函数
void AMP_CPPPlayerController::OnPickupCountChanged(int Count)
{
	// 判断 UMG 界面对象是否有效
	if (!IsValid(PickupCountWidget)) return;
	
	// 为 UI界面设置，拾取数数量
	PickupCountWidget->SetPickupCount(Count);
}
