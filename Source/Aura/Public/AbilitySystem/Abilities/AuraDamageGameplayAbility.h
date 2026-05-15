// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& Array) const;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
	float GetDamageByDamageType(int32 InLevel,const FGameplayTag& DamageType) const;
};
