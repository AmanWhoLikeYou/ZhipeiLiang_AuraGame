// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual int32 GetPlayerLevel() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetMotionWarpingTargetLocation(const FVector& TargetLocation);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitReactMontage() const;
	
	virtual void Die() = 0;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatarActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetAttackMontages() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffectNiagara() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTagsMontageByMontageTag(const FGameplayTag& MontageTag) const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void IncrementMinionCount(const int32 Amount);
};
