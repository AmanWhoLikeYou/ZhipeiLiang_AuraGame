// Zhipei Liang


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
	
	
	if (GetAuraASC()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetAuraASC()->OnAbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

	}
		
	GetAuraASC()->OnEffectAppliedDelegate.AddLambda(
	[this](const FGameplayTagContainer& EffectTagsAssetTagsContainer)
	{
	for (const FGameplayTag& Tag : EffectTagsAssetTagsContainer)
	{
		FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
		if (Tag.MatchesTag(MessageTag))
		{
			FUIWidgetRow* FoundRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable ,Tag);
			MessageWidgetRowDelegate.Broadcast(FoundRow ? *FoundRow : FUIWidgetRow());
		}
	}
	});
	
	GetAuraASC()->OnAbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
	
	
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
}



void UOverlayWidgetController::OnXPChanged(const int32 NewXP) 
{
	AAuraPlayerState* AuraPS = GetAuraPS();
	const ULevelUpInfo* LevelUpInfo = AuraPS->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("LevelUpInfo is not set in AuraPlayerState"));

	const int32 Level = LevelUpInfo->FindLevelFromXP(NewXP);
	float XPBarPercent = 0.f;
	if (LevelUpInfo->LevelUpInfos.IsValidIndex(Level))
	{
		const int32 LastLevelXPRequirement = LevelUpInfo->LevelUpInfos[Level - 1].LevelUpRequirementXP;
		const int32 NextLevelXPRequirement = LevelUpInfo->LevelUpInfos[Level].LevelUpRequirementXP;
		
		const int32 LevelXPSection = NextLevelXPRequirement - LastLevelXPRequirement;
		const int32 CurrentLevelXPProgress = NewXP - LastLevelXPRequirement;
		
		XPBarPercent = LevelXPSection > 0 ? static_cast<float>(CurrentLevelXPProgress) / LevelXPSection : 0.f;
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
	else
	{
		XPBarPercent = 1.f;
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& NowInputTag,
	const FGameplayTag& PreviousInputTag, const FGameplayTag& StatusTag) const
{
	
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo CloseUIAbilityInfo;
	CloseUIAbilityInfo.StatusTag = Tags.Abilities_Status_Unlocked;
	CloseUIAbilityInfo.AbilityTag = Tags.Abilities_None;
	CloseUIAbilityInfo.InputTag = PreviousInputTag;
	AbilityInfoDelegate.Broadcast(CloseUIAbilityInfo);
	
	FAuraAbilityInfo NewUIAbilityInfo = AbilityInfoDataAsset->FindAbilityInfoFromTag(AbilityTag);
	NewUIAbilityInfo.StatusTag = StatusTag;
	NewUIAbilityInfo.AbilityTag = AbilityTag;
	NewUIAbilityInfo.InputTag = NowInputTag;
	AbilityInfoDelegate.Broadcast(NewUIAbilityInfo);
}
