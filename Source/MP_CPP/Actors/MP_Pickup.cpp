// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_Pickup.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/MP_Player.h"
#include "Player/MP_PlayerState.h"


// Sets default values
AMP_Pickup::AMP_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bNetLoadOnClient = true;
	bReplicates = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	// The component that defines the transform (location, rotation, scale) of this Actor in the world, all other components must be attached to this one somehow
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetIsReplicated(true);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMP_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMP_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMP_Pickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 只在服务器上运行
	if (!OtherActor->HasAuthority()) return;
	
	// 检查 OtherActor 对象是否实现接口
	if (!OtherActor->Implements<UMP_Player>()) return;
	
	// 为角色增加拾取物数量
	// IMP_Player::Execute_IncrementPickupCount(OtherActor);
	
	// 为角色增加健康值
	IMP_Player::Execute_IncreaseHealth(OtherActor, HealthValue);
	
	// 增加拾取物数量
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!IsValid(OtherCharacter)) return;
	
	AMP_PlayerState* MP_PlayerState = Cast<AMP_PlayerState>(OtherCharacter->GetPlayerState());
	if (!IsValid(MP_PlayerState)) return;
	
	MP_PlayerState->SetNumPickups(MP_PlayerState->GetNumPickups() + 1);
	
	// 销毁拾取物
	Destroy();
}

