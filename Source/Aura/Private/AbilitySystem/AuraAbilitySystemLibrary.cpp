// Zhipei Liang


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuAWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			
			return Cast<UOverlayWidgetController>(HUD->GetOverlayWidgetController(WCParams));
		}
	}
	return nullptr;
}

UAttributeMenuAWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			
			return Cast<UAttributeMenuAWidgetController>(HUD->GetAttributeMenuWidgetController(WCParams));
		}
	}
	return nullptr;
}
