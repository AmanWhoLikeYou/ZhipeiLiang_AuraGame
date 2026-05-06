// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
struct FGameplayTag;
struct FGameplayTagContainer;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	
	
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);
	
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY()
	FGameplayTag CooldownTag;
	
	void HandleCooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void HandleActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& , FActiveGameplayEffectHandle ActiveHandle);
};
