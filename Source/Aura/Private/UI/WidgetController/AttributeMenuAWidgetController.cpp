// Zhipei Liang


#include "UI/WidgetController/AttributeMenuAWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuAWidgetController::BroadcastInitialValues()
{

	check(DA_AttributeInfo);
	
	for (auto& Pair : GetAuraAS()->TagsToAttribute)
	{
		BroadcastAttributesInfo(Pair.Key, Pair.Value());
	}
	
	OnAttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
	OnSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void UAttributeMenuAWidgetController::BindCallbacksToDependencies()
{

	check(DA_AttributeInfo);
	
	for (auto& Pair : GetAuraAS()->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributesInfo(Pair.Key, Pair.Value());
		});
	}
	
	AAuraPlayerState* AuraPS = GetAuraPS();
	AuraPS->OnAttributePointsChangedDelegate.AddLambda([this](const int32 NewAttributePoints)
	{
		OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
	});
	AuraPS->OnSpellPointsChangedDelegate.AddLambda([this](const int32 NewSpellPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
	});
}

void UAttributeMenuAWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = GetAuraASC();
	AuraASC->UpgradeAttribute(AttributeTag);
}


void UAttributeMenuAWidgetController::BroadcastAttributesInfo(const FGameplayTag& AttributeTag,
                                                              const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = DA_AttributeInfo->FindAttributeInfoFromTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangedDelegate.Broadcast(AttributeInfo);
}
