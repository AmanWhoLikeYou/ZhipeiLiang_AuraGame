// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignatureconst, bool, bSpendSpellPointsButtonEnabled, bool, bEquipButtonEnabled, 
	FString, Description, FString, NextLevelDescription );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityTypeTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassigned, const FGameplayTag&, AbilityTypeTag);

/**
 * 
 */
struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};


UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnCharacterStatChangedSignature OnSpellPointsChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FSpellGlobeSelectedSignatureconst OnSpellGlobeSelectedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipSelectionDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassigned SpellGlobeReassignedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendSpellPointButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void GlobeDeselected();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& GlobeInputTag, const FGameplayTag& AbilityTypeTag);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& NowInputTag, const FGameplayTag& PreviousInputTag, const FGameplayTag& StatusTag);
private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, int32 SpellPointsAmount, bool& bSpendSpellPointsButtonEnabled, bool& bEquipButtonEnabled);
	FSelectedAbility CurrentlySelectedAbility  = {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPointsAmount = 0;
	
	bool bWaitingForEquipSelection = false;
	
	FGameplayTag SelectedSlot;
};
