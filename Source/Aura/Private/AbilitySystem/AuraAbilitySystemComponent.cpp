// Zhipei Liang


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::HandleEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
				GiveAbility(AbilitySpec);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilityGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupPassiveAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{ 
			AbilitySpecInputReleased(AbilitySpec);
		}
		
	}
}

void UAuraAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Warning, TEXT("ForEachAbility delegate execution failed for ability %s"), *AbilitySpec.Ability->GetName());
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilityGivenDelegate.Broadcast(this);
	}

}

void UAuraAbilitySystemComponent::HandleEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer EffectTagsAssetTagsContainer;
	EffectSpec.GetAllAssetTags(EffectTagsAssetTagsContainer);
	
	OnEffectAppliedDelegate.Broadcast(EffectTagsAssetTagsContainer);
}
