
#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Interaction/EnemyInterface.h"


// 构造函数说明：在构造时启用了网络复制（如果未来需要在服务器/客户端间同步控制器状态）
AAuraPlayerController::AAuraPlayerController()
{
	// Make this controller replicate by default
	bReplicates = true;
}

// PlayerTick 说明：每帧检测鼠标下的 Actor，如果 Actor 实现了 IEnemyInterface 则进行高亮/取消高亮处理。
// 用途：用于实现鼠标悬停高亮敌人的交互反馈。
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
    
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
    
	if (!CursorHitResult.bBlockingHit) return;
    
	AActor* HitActor = CursorHitResult.GetActor();
    
	LastEnemy = CurrentEnemy;
	CurrentEnemy = Cast<IEnemyInterface>(HitActor);
    
	if (LastEnemy ==nullptr)
	{
		if (CurrentEnemy)
		{
			CurrentEnemy->HightlightActor();
		}
	}
	else if (LastEnemy != CurrentEnemy)
	{
		LastEnemy->UnHightlightActor();
        
		if (CurrentEnemy)
		{
			CurrentEnemy->HightlightActor();
		}
	}
    
}

// BeginPlay 说明：
// - 将设置的 Enhanced Input Mapping Context 添加到本地玩家子系统，从而启用 InputAction 的响应
// - 显示鼠标并设置输入模式为 GameAndUI（允许鼠标与游戏 UI 交互）
// 注意：使用 check(AuraContext) 确保在编辑器中已赋值映射上下文
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
    
	UEnhancedInputLocalPlayerSubsystem *LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	LocalPlayerSubsystem->AddMappingContext(AuraContext, 0);
    
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
    
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
    
}

// SetupInputComponent 说明：把定义的 MoveAction 绑定到控制器的 Move 函数上，使用 Enhanced Input 的绑定方式
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
    
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    
    
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    
}

// Move 说明：读取 Enhanced Input 的二维向量（通常来自 WASD / 摇杆），
// 将其转换为基于控制器朝向的世界方向，并向受控 Pawn 添加移动输入。
// 这样玩家朝向摄像机方向移动而不是固定世界轴向。
void AAuraPlayerController::Move(const FInputActionValue& InputValue)
{
	const FVector2D MoveVector = InputValue.Get<FVector2D>();
    
    
	const FRotator ControlRot = GetControlRotation();
	const FRotator YawRotation(0, ControlRot.Yaw, 0);
    
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MoveVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MoveVector.X);
	}
    
}
