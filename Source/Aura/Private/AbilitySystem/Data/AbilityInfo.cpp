// Zhipei Liang


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoFromTag(const FGameplayTag& AbilityTag, const bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInformation)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Warning, TEXT("AbilityInfo with tag %s not found in %s"), *AbilityTag.ToString(), *GetName());
	}
	
	return FAuraAbilityInfo();
}
