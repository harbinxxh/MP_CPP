// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_PickupCountWidget.h"

#include "Components/TextBlock.h"

// 设置拾取数量的函数
void UMP_PickupCountWidget::SetPickupCount(int32 Count) const
{
	Text_PickupCount->SetText(FText::AsNumber(Count));
}
