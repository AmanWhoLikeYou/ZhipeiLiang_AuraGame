// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UBehaviorTree;
class AAuraAIController;
enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	// Enemy Interface
	virtual void HightlightActor() override;
	virtual void UnHightlightActor() override;

	//CombatInterface
	virtual int32 GetPlayerLevel() const override;
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* Target) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged; 
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged; 
	
	
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere ,Category="Combat")	
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")	
	float LifeSpan = 5.f;
	
	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
protected:
	
	virtual void InitAbilityActorInfo() override;
	
	
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category="Character Class Default")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category="Character Class Default")	
	ECharacterClass CharacterClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
	
	UPROPERTY(EditAnywhere,Category= "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	

	
};
