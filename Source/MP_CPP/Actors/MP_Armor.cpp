// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_Armor.h"

#include "Components/SphereComponent.h"
#include "Interaction/MP_Player.h"


// Sets default values
AMP_Armor::AMP_Armor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	// The component that defines the transform (location, rotation, scale) of this Actor in the world, all other components must be attached to this one somehow
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetIsReplicated(true);
	
	BootMesh_L = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_L");
	BootMesh_L->SetupAttachment(RootComponent);
	BootMesh_L->SetIsReplicated(true);
	
	BootMesh_R = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_R");
	BootMesh_R->SetupAttachment(RootComponent);
	BootMesh_R->SetIsReplicated(true);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);
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

/** 现在 Armor Actor 不再依赖玩家角色 Character, 而是依赖更抽象的玩家接口。*/ 
void AMP_Armor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 只在服务器上运行
	if (!OtherActor->HasAuthority()) return;
	
	// 检查 OtherActor 对象是否实现接口
	if (OtherActor->Implements<UMP_Player>())
	{
		// 调用接口
		USkeletalMeshComponent* Mesh = IMP_Player::Execute_GetSkeletalMesh(OtherActor);
		// 骨骼绑定
		SphereMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Head");
		BootMesh_L->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Foot_L");
		BootMesh_R->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "S_Foot_R");
		
		IMP_Player::Execute_GrantArmor(OtherActor, ArmorValue);
	}
}