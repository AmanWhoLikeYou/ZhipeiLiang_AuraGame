// Zhipei Liang


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::HandleEffectApplied);
}

void UAuraAbilitySystemComponent::HandleEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                      const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer EffectTagsAssetTagsContainer;
	EffectSpec.GetAllAssetTags(EffectTagsAssetTagsContainer);
	
	OnEffectAppliedDelegate.Broadcast(EffectTagsAssetTagsContainer);
}
