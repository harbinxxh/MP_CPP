// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_Armor.h"

#include "Components/SphereComponent.h"
#include "Interaction/MP_Player.h"


// Sets default values
AMP_Armor::AMP_Armor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//1、创建默认场景组件，并把它设置为根组件
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	//2、创建球体碰撞组件
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	//3、将球体碰撞组件附加到根组件
	SphereCollision->SetupAttachment(RootComponent);
	
	// 创建静态网格组件
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	// 将球体静态网格组件附加到根组件
	SphereMesh->SetupAttachment(RootComponent);
	// 启用组件复制
	SphereMesh->SetIsReplicated(true);
	
	BootMesh_L = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_L");
	BootMesh_L->SetupAttachment(RootComponent);
	BootMesh_L->SetIsReplicated(true);
	
	BootMesh_R = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_R");
	BootMesh_R->SetupAttachment(RootComponent);
	BootMesh_R->SetIsReplicated(true);
	

}

// Called when the game starts or when spawned
void AMP_Armor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMP_Armor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// 当此角色与其他角色重叠时触发的事件，例如玩家走入触发器。
void AMP_Armor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 只在服务器上运行
	if (!OtherActor->HasAuthority()) return;
	
	/** 现在 Armor Actor 不再依赖玩家角色 Character, 而是依赖更抽象的玩家接口。*/ 
	// 检查 OtherActor 对象是否实现接口
	if (OtherActor->Implements<UMP_Player>())
	{
		// 调用接口
		USkeletalMeshComponent* Mesh = IMP_Player::Execute_GetSkeletalMesh(OtherActor);
		
		/**
		 * 将此组件附加到另一个场景组件上，可选择性地连接到命名的接口。
		 * 无论组件是否已注册，均可调用此方法，但建议在构造函数中或未注册时使用 SetupAttachment。
		 */
		SphereMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Head");
		BootMesh_L->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Foot_L");
		BootMesh_R->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Foot_R");
		
		IMP_Player::Execute_GrantArmor(OtherActor, ArmorValue);
	}
}