// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

class UAuraWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	UObject* WidgetController;
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* NewWidgetController);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void AfterWidgetControllerSet();
};
