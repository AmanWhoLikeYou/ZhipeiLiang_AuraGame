// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UAuraWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:

	UAuraWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAuraWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	UAuraWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	
	
	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet);
	
protected:
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	//OverlayWidgetController
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAuraWidgetController> OverlayWidgetController;
	
	//AttributeMenuController
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> AttributeMenuWidgetControllerClass;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAuraWidgetController> AttributeMenuWidgetController;
	
	//SpellMenuController
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> SpellMenuWidgetControllerClass;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAuraWidgetController> SpellMenuWidgetController;
};
