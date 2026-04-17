// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidgetClass = nullptr;;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UAuraUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, const FUIWidgetRow&, MessageWidgetRow);


UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FOnMessageWidgetRowSignature MessageWidgetRowDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	
	template<typename T>
	T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(const UDataTable* DataTable ,const FGameplayTag& Tag) const
{
	return DataTable ? DataTable->FindRow<T>(Tag.GetTagName(), TEXT("")) : nullptr;
}

