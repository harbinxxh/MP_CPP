// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MP_CPPPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UMP_PickupCountWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AMP_CPPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	virtual void OnRep_PlayerState() override;

private:
	
	// 需要实例化的类型
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMP_PickupCountWidget> PickupCountWidgetClass;
	
	// 创建后的实例化对象
	UPROPERTY()
	TObjectPtr<UMP_PickupCountWidget> PickupCountWidget;
	
	UFUNCTION()
	void OnPickupCountChanged(int Count);
};
