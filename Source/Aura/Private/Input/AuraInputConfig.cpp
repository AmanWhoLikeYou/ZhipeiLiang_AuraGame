// Zhipei Liang


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotfound) const
{
	for (const FAuraInputAction& InputAction : AbilityInputActions)
	{
		if (InputAction.InputTag == InputTag)
		{
			return InputAction.InputAction;
		}
	}
	
	if (bLogNotfound)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityInputConfig: Cannot find input action for tag %s"), *InputTag.ToString());
	}
	
	return nullptr;
}
