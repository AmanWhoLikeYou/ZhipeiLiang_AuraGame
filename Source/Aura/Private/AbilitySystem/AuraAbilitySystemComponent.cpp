// Zhipei Liang


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

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
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
				GiveAbility(AbilitySpec);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilityGivenDelegate.Broadcast();
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

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromAbilitySpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromAbilitySpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock AbilityListLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability)
		{
			for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
			{
				if (Tag.MatchesTag(AbilityTag))
				{
					return &AbilitySpec;
				}
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& AuraAbilityInfo : AbilityInfo->AbilitiesInformation)
	{
		if (!AuraAbilityInfo.AbilityTag.IsValid()) continue;
		if (AuraAbilityInfo.LevelRequirement > Level) continue;
		if (GetSpecFromAbilityTag(AuraAbilityInfo.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AuraAbilityInfo.AbilityClass, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(AuraAbilityInfo.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		FGameplayTag PreviousInputTag = GetInputTagFromAbilitySpec(*AbilitySpec);
		FGameplayTag StatusTag = GetStatusTagFromAbilityTag(AbilityTag);
		bool bIsStatusValid = StatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked) || StatusTag.MatchesTagExact(Tags.Abilities_Status_Equipped);
		if (bIsStatusValid)
		{
			ClearAbilitiesOfInputTag(InputTag);
			ClearInputTagsFromAbilitySpec(AbilitySpec);
			
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(InputTag);
			
			if (StatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Tags.Abilities_Status_Unlocked);
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag, InputTag, PreviousInputTag,Tags.Abilities_Status_Equipped);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& NowInputTag, const FGameplayTag& PreviousInputTag, const FGameplayTag& StatusTag)
{
	OnAbilityEquippedDelegate.Broadcast(AbilityTag, NowInputTag, PreviousInputTag, StatusTag);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
                                                              FString& OutNextLevelDescription)
{
	if (FGameplayAbilitySpec *AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FAuraAbilityInfo AuraAbilityInfo = AbilityInfo->FindAbilityInfoFromTag(AbilityTag);
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();

	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AuraAbilityInfo.LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UAuraAbilitySystemComponent::ClearInputTagsFromAbilitySpec(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag InputTag = GetInputTagFromAbilitySpec(*AbilitySpec);
	if (InputTag.IsValid())
	{
		AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
	}
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock AbilityListLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(AbilitySpec, InputTag))
		{
			ClearInputTagsFromAbilitySpec(&AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& InputTag)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(InputTag))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		
		FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromAbilitySpec(*AbilitySpec);
		
		if (StatusTag.MatchesTagExact(Tags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Tags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(Tags.Abilities_Status_Unlocked);
		}
		else if (StatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked) || StatusTag.MatchesTagExact(Tags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level += 1;
		}
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		MarkAbilitySpecDirty(*AbilitySpec);
		ClientUpdateAbilityStatus(AbilityTag, GetStatusTagFromAbilitySpec(*AbilitySpec), AbilitySpec->Level);
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.Instigator = GetAvatarActor();
	Payload.Target = GetAvatarActor();
	Payload.EventMagnitude = 1;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,int32 AbilityLevel)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilityGivenDelegate.Broadcast();
	}

}

void UAuraAbilitySystemComponent::HandleEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer EffectTagsAssetTagsContainer;
	EffectSpec.GetAllAssetTags(EffectTagsAssetTagsContainer);
	
	OnEffectAppliedDelegate.Broadcast(EffectTagsAssetTagsContainer);
}
