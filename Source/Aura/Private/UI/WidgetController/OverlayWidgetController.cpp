// Zhipei Liang


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
	
	
	if (UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->OnAbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);

		}
		
		AuraASC->OnEffectAppliedDelegate.AddLambda(
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
	}
	
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPS->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPS->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC) const
{
	if (!AuraASC->bStartupAbilitiesGiven) return;
	
	FForEachAbility ForEachAbilityDelegate;
	ForEachAbilityDelegate.BindLambda([this,AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAbilityInfoFromTag(AuraASC->GetAbilityTagFromAbilitySpec(AbilitySpec));
		
		AbilityInfo.InputTag = AuraASC->GetInputTagFromAbilitySpec(AbilitySpec);
		
		AbilityInfoDelegate.Broadcast(AbilityInfo);
	});
	
    AuraASC->ForEachAbility(ForEachAbilityDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
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
