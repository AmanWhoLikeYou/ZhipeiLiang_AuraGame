// Zhipei Liang


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

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
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this,
		&UOverlayWidgetController::HandleHealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this,
		&UOverlayWidgetController::HandleMaxHealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this,
		&UOverlayWidgetController::HandleManaChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this,
		&UOverlayWidgetController::HandleMaxManaChanged);
}

void UOverlayWidgetController::HandleHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
