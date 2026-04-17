// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap UMETA(DisplayName = "Apply On Begin Overlap"),
	ApplyOnEndOverlap UMETA(DisplayName = "Apply On End Overlap"),
	DoNotApply UMETA(DisplayName = "Do Not Apply")
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap UMETA(DisplayName = "Remove On End Overlap"),
	DoNotRemove UMETA(DisplayName = "Do Not Remove")
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// 构造函数：设置此 Actor 的默认属性
	AAuraEffectActor();


protected:
	// 在游戏开始或该 Actor 被生成时调用
	virtual void BeginPlay() override;

	// 将指定的 GameplayEffectClass 应用到目标 Actor
	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	// 当有 Actor 与此 EffectActor 开始重叠时调用（Begin Overlap）
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// 当有 Actor 与此 EffectActor 结束重叠时调用（End Overlap）
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	bool bDestroyOnEffectRemoval = false;
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	// 瞬时效果：可以配置为多个 GameplayEffect 类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InstantGameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	// 持续效果：可以配置为多个 GameplayEffect 类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy DurationGameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	// 永久/无限时长效果：可以配置为多个 GameplayEffect 类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteGameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectRemovalPolicy InfiniteGameplayEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.f;
};
