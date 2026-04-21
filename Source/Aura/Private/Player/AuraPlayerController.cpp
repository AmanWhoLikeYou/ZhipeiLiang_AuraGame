
#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"


// 构造函数说明：在构造时启用了网络复制（如果未来需要在服务器/客户端间同步控制器状态）
AAuraPlayerController::AAuraPlayerController()
{
	// Make this controller replicate by default
	bReplicates = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

// PlayerTick 说明：每帧检测鼠标下的 Actor，如果 Actor 实现了 IEnemyInterface 则进行高亮/取消高亮处理。
// 用途：用于实现鼠标悬停高亮敌人的交互反馈。
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
	AutoRun();
}

// BeginPlay 说明：
// - 将设置的 Enhanced Input Mapping Context 添加到本地玩家子系统，从而启用 InputAction 的响应
// - 显示鼠标并设置输入模式为 GameAndUI（允许鼠标与游戏 UI 交互）
// 注意：使用 check(AuraContext) 确保在编辑器中已赋值映射上下文
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
    
	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (LocalPlayerSubsystem)
	{
		LocalPlayerSubsystem->AddMappingContext(AuraContext, 0);
	}
    
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
    
	UAuraInputComponent* AuraEnhancedInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    
    
	AuraEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraEnhancedInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, 
		&AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
	
	
}

void AAuraPlayerController::CursorTrace()
{
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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentEnemy == nullptr ? false : true;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputReleased(InputTag);
		return;
	}
	if (bTargeting)
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputReleased(InputTag);
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), ControlledPawn->GetActorLocation(), CachedDestination))
			{
				if (NavPath->IsValid())
				{
					SplineComponent->ClearSplinePoints();
					for (const auto& PathPoint : NavPath->PathPoints)
					{
						SplineComponent->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
						DrawDebugSphere(GetWorld(), PathPoint, 25.f, 12, FColor::Green, false, 2.f);
					}
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputReleased(InputTag);
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputReleased(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			CachedDestination = HitResult.ImpactPoint;
		}
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(Direction, 1.0f);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())	
	{
		const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction, 1.0f);
		
		ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
		if (ControlledCharacter)
		{
			UCapsuleComponent* Capsule = ControlledCharacter->GetCapsuleComponent();
			const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();

			const FVector FeetLocation =
				ControlledCharacter->GetActorLocation() - FVector(0.f, 0.f, HalfHeight);
			
			const float DistanceToTarget = FVector::Dist(FeetLocation, CachedDestination);
			if (DistanceToTarget <= AutoRunAcceptanceRadius)
			{
				bAutoRunning = false;
			}
		}
	}
}
