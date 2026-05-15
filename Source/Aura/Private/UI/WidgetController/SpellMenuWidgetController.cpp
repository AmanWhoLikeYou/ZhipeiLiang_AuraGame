// Zhipei Liang


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (GetAuraASC())
	{
		GetAuraASC()->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (CurrentlySelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				CurrentlySelectedAbility.StatusTag = StatusTag;
				bool bSpendSpellPointsButtonEnabled = false;
				bool bEquipButtonEnabled = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPointsAmount, bSpendSpellPointsButtonEnabled, bEquipButtonEnabled);
				
				FString Description = FString();
				FString NextLevelDescription = FString();
				if (GetAuraASC())
				{
					GetAuraASC()->GetDescriptionsByAbilityTag(CurrentlySelectedAbility.AbilityTag, Description, NextLevelDescription);
				}
				OnSpellGlobeSelectedDelegate.Broadcast(bSpendSpellPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
			}
			
			if (AbilityInfoDataAsset)
			{
				FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAbilityInfoFromTag(AbilityTag);
				AbilityInfo.AbilityTag = AbilityTag;
				AbilityInfo.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(AbilityInfo);
			}
		});
	}
	
	AAuraPlayerState* AuraPS = GetAuraPS();
	AuraPS->OnSpellPointsChangedDelegate.AddLambda([this](const int32 NewSpellPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
		CurrentSpellPointsAmount = NewSpellPoints;
		
		bool bSpendSpellPointsButtonEnabled = false;
		bool bEquipButtonEnabled = false;
		ShouldEnableButtons(CurrentlySelectedAbility.StatusTag, CurrentSpellPointsAmount, bSpendSpellPointsButtonEnabled, bEquipButtonEnabled);
		
		FString Description = FString();
		FString NextLevelDescription = FString();
		if (GetAuraASC())
		{
			GetAuraASC()->GetDescriptionsByAbilityTag(CurrentlySelectedAbility.AbilityTag, Description, NextLevelDescription);
		}
		OnSpellGlobeSelectedDelegate.Broadcast(bSpendSpellPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
	});
	
	GetAuraASC()->OnAbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityTypeTag = AbilityInfoDataAsset->FindAbilityInfoFromTag(CurrentlySelectedAbility.AbilityTag).AbilityTypeTag;
		StopWaitForEquipSelectionDelegate.Broadcast(SelectedAbilityTypeTag);
		bWaitingForEquipSelection = false;
	}
	
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	const bool bIsTagValid = AbilityTag.IsValid();
	const bool bTagIsNone = AbilityTag.MatchesTag(Tags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bIsSpecValid = AbilitySpec != nullptr;
	
	FGameplayTag AbilityStatusTag;
	
	if (!bIsTagValid || bTagIsNone || !bIsSpecValid)
	{
		AbilityStatusTag = Tags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatusTag = GetAuraASC()->GetStatusTagFromAbilitySpec(*AbilitySpec);
	}
	 
	CurrentlySelectedAbility.AbilityTag = AbilityTag;
	CurrentlySelectedAbility.StatusTag = AbilityStatusTag;
	
	bool bSpendSpellPointsButtonEnabled = false;
	bool bEquipButtonEnabled = false;
	
	ShouldEnableButtons(AbilityStatusTag, GetAuraPS()->GetSpellPoints(), bSpendSpellPointsButtonEnabled, bEquipButtonEnabled);
	
	FString Description = FString();
	FString NextLevelDescription = FString();
	if (GetAuraASC())
	{
		GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	}
	OnSpellGlobeSelectedDelegate.Broadcast(bSpendSpellPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendSpellPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(CurrentlySelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::GlobeDeselected()
{
	
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityTypeTag = AbilityInfoDataAsset->FindAbilityInfoFromTag(CurrentlySelectedAbility.AbilityTag).AbilityTypeTag;
		StopWaitForEquipSelectionDelegate.Broadcast(SelectedAbilityTypeTag);
		bWaitingForEquipSelection = false;
	}
	
	CurrentlySelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	CurrentlySelectedAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	
	OnSpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());


}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfoDataAsset->FindAbilityInfoFromTag(CurrentlySelectedAbility.AbilityTag).AbilityTypeTag;
	
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	
	// const FGameplayTag SelectedAbilityStatusTag = CurrentlySelectedAbility.StatusTag;
	const FGameplayTag SelectedAbilityStatusTag = GetAuraASC()->GetStatusTagFromAbilityTag(CurrentlySelectedAbility.AbilityTag);
	if (SelectedAbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag(CurrentlySelectedAbility.AbilityTag);
	}
	
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& GlobeInputTag, const FGameplayTag& AbilityTypeTag)
{
	if (!bWaitingForEquipSelection) return;
	
	const FGameplayTag SelectedAbilityAbilityTypeTag = AbilityInfoDataAsset->FindAbilityInfoFromTag(CurrentlySelectedAbility.AbilityTag).AbilityTypeTag;
	if (!SelectedAbilityAbilityTypeTag.MatchesTagExact(AbilityTypeTag))
	{
		return;
	}
	
	GetAuraASC()->ServerEquipAbility(CurrentlySelectedAbility.AbilityTag, GlobeInputTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& NowInputTag,
	const FGameplayTag& PreviousInputTag, const FGameplayTag& StatusTag)
{
	bWaitingForEquipSelection = false;
	
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
	
	StopWaitForEquipSelectionDelegate.Broadcast(AbilityInfoDataAsset->FindAbilityInfoFromTag(AbilityTag).AbilityTypeTag);
	
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselected();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, int32 SpellPointsAmount,
                                                     bool& bSpendSpellPointsButtonEnabled, bool& bEquipButtonEnabled)
{
	FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	
	bSpendSpellPointsButtonEnabled = false;
	bEquipButtonEnabled = false;
	if (AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Eligible))
	{
		if (SpellPointsAmount > 0)
		{
			bSpendSpellPointsButtonEnabled = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Unlocked))
	{
		bEquipButtonEnabled = true;
		if (SpellPointsAmount > 0)
		{
			bSpendSpellPointsButtonEnabled = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(Tags.Abilities_Status_Equipped))
	{
		bEquipButtonEnabled = true;
		if (SpellPointsAmount > 0)
		{
			bSpendSpellPointsButtonEnabled = true;
		}
	}
}
