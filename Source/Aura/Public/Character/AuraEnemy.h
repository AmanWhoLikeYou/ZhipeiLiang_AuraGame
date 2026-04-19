// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	
	virtual void BeginPlay() override;
	
	// Enemy Interface
	virtual void HightlightActor() override;
	virtual void UnHightlightActor() override;

	//CombatInterface
	virtual int32 GetPlayerLevel() const override;
protected:
	
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category="Character Class Default")
	int32 Level = 1;
};
