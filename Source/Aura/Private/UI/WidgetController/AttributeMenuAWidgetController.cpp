// Zhipei Liang


#include "UI/WidgetController/AttributeMenuAWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuAWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(DA_AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttribute)
	{
		BroadcastAttributesInfo(Pair.Key, Pair.Value());
	}
	
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
}

void UAttributeMenuAWidgetController::BroadcastAttributesInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = DA_AttributeInfo->FindAttributeInfoFromTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangedDelegate.Broadcast(AttributeInfo);
}
