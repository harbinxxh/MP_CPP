// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MP_PickupCountWidget.generated.h"

class UTextBlock;

/**
 * 在玩家控制器中添加这个 Widget 控制
 */
UCLASS()
class MP_CPP_API UMP_PickupCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPickupCount(int32 Count) const;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_PickupCount;
};
