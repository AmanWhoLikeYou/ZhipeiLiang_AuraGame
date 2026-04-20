// Zhipei Liang


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, float bLogNotfound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return AttributeInfo;
		}
	}
	
	if (bLogNotfound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute with tag %s not found in AttributeInfo data asset."), *AttributeTag.ToString());
	}
	
	return FAuraAttributeInfo();
}

