// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MP_Armor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class MP_CPP_API AMP_Armor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_Armor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 当此角色与其他角色重叠时触发的事件，例如玩家走入触发器
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SphereMesh; // 静态网格组件
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BootMesh_L; // 静态网格组件
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BootMesh_R; // 静态网格组件
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision; // 球形碰撞提组件
	
	UPROPERTY(EditAnywhere)
	float ArmorValue = 100.0f; // 护甲值
};
