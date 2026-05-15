// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	
	//CombatInterface
	virtual int32 GetPlayerLevel_Implementation() const override;
	
	
	//PlayerInterface
	virtual void AddXP_Implementation(int32 AddXPAmount) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetLevelFromXP_Implementation(int32 XP) const override;
	virtual int32 GetAttributePointReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 AddLevelAmount) override;
	virtual void AddToAttributePoints_Implementation(int32 AddAttributePointAmount) override;
	virtual void AddToSpellPoints_Implementation(int32 AddSpellPointAmount) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
protected:
	virtual void InitAbilityActorInfo() override;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TobDownCameraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
};
