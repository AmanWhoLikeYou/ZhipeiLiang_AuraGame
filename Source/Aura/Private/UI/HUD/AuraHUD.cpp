// Zhipei Liang


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"

UAuraWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UAuraWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAuraWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAuraWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UAuraWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<UAuraWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* ASC,
                           UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is nullptr. Please assign it in the editor."));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr. Please assign it in the editor."));
	
	FWidgetControllerParams WCParams(PlayerController, PlayerState, ASC, AttributeSet);
	UAuraWidgetController* OverlayWC = GetOverlayWidgetController(WCParams);
	
	OverlayWidget = CreateWidget<UAuraUserWidget>(PlayerController, OverlayWidgetClass);
	OverlayWidget->SetWidgetController(OverlayWC);
	
	OverlayWC->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
	

}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}

