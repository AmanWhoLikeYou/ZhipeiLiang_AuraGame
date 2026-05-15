// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffctAppliedDelegate, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/,int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEauiped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*NowInputTag*/, 
	const FGameplayTag& /*PreviousInputTag*/, const FGameplayTag& /*StatusTag*/);
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
	FAbilityStatusChanged OnAbilityStatusChangedDelegate;
	FAbilityEauiped OnAbilityEquippedDelegate;
	
	bool bStartupAbilitiesGiven = false;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	void AbilityInputPressed(const FGameplayTag& InputTag);
	
	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	void UpdateAbilityStatuses(int32 Level);
	
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	
	UFUNCTION(Server, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& NowInputTag, const FGameplayTag& PreviousInputTag, const FGameplayTag& StatusTag);
	
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	void ClearInputTagsFromAbilitySpec(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilitiesOfInputTag(const FGameplayTag& InputTag);
	static bool AbilityHasInputTag(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag);
protected:
	
	UFUNCTION(Client, Reliable)
	void HandleEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	
	virtual void OnRep_ActivateAbilities() override;
};
