// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuAWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
class UAttributeInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfocChangedSignature, const FAuraAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UAttributeMenuAWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeInfocChangedSignature OnAttributeInfoChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnCharacterStatChangedSignature OnAttributePointsChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnCharacterStatChangedSignature OnSpellPointsChangedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> DA_AttributeInfo;
	
private:
	
	void BroadcastAttributesInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
