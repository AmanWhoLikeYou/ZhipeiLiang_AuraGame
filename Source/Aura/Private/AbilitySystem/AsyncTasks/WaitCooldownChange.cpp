// Zhipei Liang


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag&  InCooldownTag)
{
	if (!AbilitySystemComponent || !InCooldownTag.IsValid())
	{
		return nullptr;
	}
	
	UWaitCooldownChange* WaitCooldownChangedTask = NewObject<UWaitCooldownChange>();
	WaitCooldownChangedTask->ASC = AbilitySystemComponent;
	WaitCooldownChangedTask->CooldownTag = InCooldownTag;
	
	//冷却消失
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChangedTask
		, &UWaitCooldownChange::HandleCooldownChanged);
	
	//冷却效果添加时
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChangedTask, &UWaitCooldownChange::HandleActiveEffectAdded);
	
	return WaitCooldownChangedTask;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::HandleCooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::HandleActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	
	if (GrantedTags.HasTagExact(CooldownTag) || AssetTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = TargetASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		
		if (TimesRemaining.Num() > 0)
		{
			float LongestTime = TimesRemaining[0];
			for (int32 Idx = 1; Idx < TimesRemaining.Num(); ++Idx)
			{
				if (TimesRemaining[Idx] > LongestTime)
				{
					LongestTime = TimesRemaining[Idx];
				}
			}
			CooldownStart.Broadcast(LongestTime);
		}
	}
}
