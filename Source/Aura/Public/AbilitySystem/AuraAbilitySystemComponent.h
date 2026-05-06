// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffctAppliedDelegate, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityGiven, UAuraAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();
	
	FOnEffctAppliedDelegate OnEffectAppliedDelegate;
	FAbilityGiven OnAbilityGivenDelegate;
	
	bool bStartupAbilitiesGiven = false;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	void AbilityInputPressed(const FGameplayTag& InputTag);
	
	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
protected:
	
	UFUNCTION(Client, Reliable)
	void HandleEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);\

	virtual void OnRep_ActivateAbilities() override;
};
