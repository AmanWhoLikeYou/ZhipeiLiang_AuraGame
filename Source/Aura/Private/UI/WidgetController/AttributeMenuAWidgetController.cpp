// Zhipei Liang


#include "UI/WidgetController/AttributeMenuAWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuAWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(DA_AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttribute)
	{
		BroadcastAttributesInfo(Pair.Key, Pair.Value());
	}
	
	OnAttributePointsChangedDelegate.Broadcast(CastChecked<AAuraPlayerState>(PlayerState)->GetAttributePoints());
	OnSpellPointsChangedDelegate.Broadcast(CastChecked<AAuraPlayerState>(PlayerState)->GetSpellPoints());
}

void UAttributeMenuAWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(DA_AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributesInfo(Pair.Key, Pair.Value());
		});
	}
	
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
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
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuAWidgetController::BroadcastAttributesInfo(const FGameplayTag& AttributeTag,
                                                              const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = DA_AttributeInfo->FindAttributeInfoFromTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangedDelegate.Broadcast(AttributeInfo);
}
