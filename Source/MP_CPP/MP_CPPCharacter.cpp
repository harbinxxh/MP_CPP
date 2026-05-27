// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_CPPCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MP_CPP.h"
#include "ShaderPrintParameters.h"
#include "Actors/MP_Actor.h"
#include "Components/MP_HealthComponent.h"
#include "Game/MP_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/MP_PlayerState.h"

AMP_CPPCharacter::AMP_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	HealthComponent = CreateDefaultSubobject<UMP_HealthComponent>("HealthComponent");
	// 启动组件复制
	HealthComponent->SetIsReplicated(true);
	
}

USkeletalMeshComponent* AMP_CPPCharacter::GetSkeletalMesh_Implementation() const
{
	return GetMesh();
}

void AMP_CPPCharacter::GrantArmor_Implementation(float ArmorAmount)
{
	Armor = ArmorAmount;
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("Armor: %f"), Armor)
		);
}

void AMP_CPPCharacter::IncrementPickupCount_Implementation()
{
	++PickupCount;
}

void AMP_CPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Look);
		
		// General Action for testing
		EnhancedInputComponent->BindAction(GeneralAction, ETriggerEvent::Started, this, &AMP_CPPCharacter::OnGeneralInput);
	}
	else
	{
		UE_LOG(LogMP_CPP, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMP_CPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMP_CPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMP_CPPCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMP_CPPCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMP_CPPCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMP_CPPCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AMP_CPPCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 3、Call DOREPLIFETIME
	// DOREPLIFETIME(ThisClass, Armor);
	// DOREPLIFETIME(ThisClass, PickupCount);
	
	// 条件复制
	DOREPLIFETIME_CONDITION(ThisClass, Armor, COND_InitialOrOwner);
	// 自定义条件复制
	DOREPLIFETIME_CONDITION(ThisClass, PickupCount, COND_Custom);
}

void AMP_CPPCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	
	// 开关变量关联复制变量
	// 如果 bReplicatePickupCount 的值发生变化，PickupCount 将会改变其复制状态。
	// 注意 - bReplicatePickupCount 的值仅在服务器端才有意义。
	DOREPLIFETIME_ACTIVE_OVERRIDE(ThisClass, PickupCount, bReplicatePickupCount);
}

void AMP_CPPCharacter::OnGeneralInput()
{
	// 切换条件复制变量值
	// bReplicatePickupCount = !bReplicatePickupCount;
	//
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 5.f,
	// FColor::Green,
	// FString::Printf(TEXT("bReplicatePickupCount: %d"), bReplicatePickupCount)
	// );
	
	
	// Server_PrintMessage("Please run this on the server");
	// Server_PrintMessage(FString());
	
	
	// 获取游戏状态
	// AMP_GameState* MP_GameState = Cast<AMP_GameState>(UGameplayStatics::GetGameState(this));
	// APlayerController* PlayerController = Cast<APlayerController>(GetController());
	// if (IsValid(MP_GameState) && IsValid(PlayerController))
	// {
	// 	FString TeamMessage = "Team";
	// 	if (MP_GameState->IsTeamOne(PlayerController))
	// 	{
	// 		TeamMessage += "One";
	// 	}
	// 	else
	// 	{
	// 		TeamMessage += "Two";
	// 	}
	// 	
	// 	GEngine->AddOnScreenDebugMessage(-1,60.f,FColor::Cyan,TeamMessage);
	// }
	
	
	// 获取玩家状态
	AMP_PlayerState* MP_PlayerState = Cast<AMP_PlayerState>(GetPlayerState());
	if (!IsValid(MP_PlayerState)) return;
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	35.f,
	FColor::Blue,
	FString::Printf(TEXT("NumPickups: %d"), MP_PlayerState->GetNumPickups()));
}

void AMP_CPPCharacter::OnRep_Armor()
{	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Orange,
	FString::Printf(TEXT("Armor: %f"), Armor)
	);
}

void AMP_CPPCharacter::OnRep_PickupCount(int32 PreviousValue)
{	
	// 显示拾取复制前的数值
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Cyan,
	FString::Printf(TEXT("Previous Pickup Count: %d"), PreviousValue)
	);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Red,
	FString::Printf(TEXT("Pickup Count: %d"), PickupCount)
	);
}

void AMP_CPPCharacter::IncreaseHealth_Implementation(float NewHealthAmount)
{
	if (IsValid(HealthComponent))
	{
		HealthComponent->SetHealth(HealthComponent->GetHealth() + NewHealthAmount);
	}
}

void AMP_CPPCharacter::OnRep_RPCDelayTimer()
{
	// if (HasAuthority())
	// {
	// 	Client_PrintMessage("This should run on the owning client.");
	// }

	if (!HasAuthority()) return;
	
	// FActorSpawnParameters SpawnParams;
	// SpawnParams.Owner = this;
	// GetWorld()->SpawnActor<AMP_Actor>(GetActorLocation(), GetActorRotation(), SpawnParams);
	
	Multicast_PrintMessage("Print this on the server and all relevant clients");
}

bool AMP_CPPCharacter::Server_PrintMessage_Validate(const FString& Message)
{
	// 只要客户端返回消息，被判断为假
	// 服务器就会踢掉客户端连接
	return !Message.IsEmpty();
}

void AMP_CPPCharacter::Server_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += Message;
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	30.f,
	FColor::Purple,
	MessageString
	);
}

void AMP_CPPCharacter::Multicast_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += Message;
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	30.f,
	FColor::Cyan,
	MessageString
	);
}

void AMP_CPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(RPCDelayTimer, this, &AMP_CPPCharacter::OnRep_RPCDelayTimer, 4.0f,false);
}

void AMP_CPPCharacter::Client_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += Message;
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	30.f,
	FColor::Yellow,
	MessageString
	);
}
