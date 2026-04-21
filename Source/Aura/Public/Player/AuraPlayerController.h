#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class IEnemyInterface;
/**
 * 
 */
// 玩家控制器：处理玩家输入、鼠标光标以及与敌人交互的高层逻辑
// 责任：
// - 在 BeginPlay 中为本地玩家添加 Enhanced Input 的 Mapping Context
// - 在 PlayerTick 中根据鼠标位置检测并高亮/取消高亮敌人
// - 在 SetupInputComponent 中绑定输入动作（例如移动）
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Controller constructor: enable replication by default
	AAuraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	// 在这里可做控制器级别的初始化（例如添加输入映射、设置鼠标显示以及输入模式）
	virtual void BeginPlay() override;
    
	// 绑定输入组件：把 Enhanced Input 的 Action 与控制器的方法绑定起来
	virtual void SetupInputComponent() override;


private:

	void CursorTrace();
	
	// Enhanced Input 的 Mapping Context：用于在本地玩家子系统中注册输入映射
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
    
	// 移动动作（通过 Enhanced Input 定义的 InputAction）
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
    
	// 移动处理函数：从输入值中读取二维向量并驱动角色移动
	void Move(const FInputActionValue& InputValue);
    
	// 上一帧检测到的敌人（用于取消高亮）
	IEnemyInterface* LastEnemy;
	// 当前帧检测到的敌人（用于高亮）
	IEnemyInterface* CurrentEnemy;
	
	
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();
	
	//鼠标移动功能
	
	void AutoRun();
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USplineComponent> SplineComponent;
};
