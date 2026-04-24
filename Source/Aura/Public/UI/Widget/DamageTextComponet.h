// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextComponet : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageTextValue(float DamageValue);
};
