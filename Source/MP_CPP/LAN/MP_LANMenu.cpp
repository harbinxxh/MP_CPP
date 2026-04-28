// Fill out your copyright notice in the Description page of Project Settings.


#include "LAN/MP_LANMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMP_LANMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);	// 仅允许用户界面响应用户输入
	GetOwningPlayer()->SetShowMouseCursor(true);// 显示鼠标光标


	Button_Host->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	Button_Join->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
}

void UMP_LANMenu::HostButtonClicked()
{
	FInputModeGameOnly InputMode;	// 仅允许玩家输入/玩家控制器响应用户输入
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	// 通过监听服务器模式打开新关卡
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, HostingLevel, true, TEXT("listen"));
}

void UMP_LANMenu::JoinButtonClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	const FString Address = TextBox_IpAddress->GetText().ToString();
	UGameplayStatics::OpenLevel(this, *Address);
}
