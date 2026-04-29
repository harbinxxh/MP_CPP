// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MP_LANMenu.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class MP_CPP_API UMP_LANMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_IpAddress;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Host;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Join;

	// 通过 SoftObject 软引用，可以指定任意关卡
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> HostingLevel;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();
};
