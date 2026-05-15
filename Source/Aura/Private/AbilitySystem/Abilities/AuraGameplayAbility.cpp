// Zhipei Liang


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsumLoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum "
	"LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum ", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </> "), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default> Spell Locked Untile Level: %d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(int32 Level) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* ManaCostGE = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo ModifierInfo : ManaCostGE->Modifiers)
		{
			if (ModifierInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(int32 Level) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		CooldownGE->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
	}
	return Cooldown;
}
