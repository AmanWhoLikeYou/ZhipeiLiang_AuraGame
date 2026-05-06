#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UAuraGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	/*CombatInterface*/
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatarActor_Implementation() override;
	
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	
	//处理死亡在服务端的逻辑
	virtual void Die() override;
	//处理死亡在服务端和客户端的逻辑
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	
	virtual UNiagaraSystem* GetBloodEffectNiagara_Implementation() const override;
	
	virtual FTaggedMontage GetTagsMontageByMontageTag_Implementation(const FGameplayTag& MontageTag ) const override;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	TMap<FGameplayTag,FName> TagToAttackSocketNameMap;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	bool bUseWeapon = true;
	
	//Minions
	int32 MinionCount = 0;
	
	virtual int32 GetMinionCount_Implementation() const override;
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override;
	
	/*CombatInterface*/
	
protected:

	bool bDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributesInitializationEffect;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffect;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffect;
	
	UPROPERTY(EditAnywhere,blueprintReadOnly,Category="Character Class Default")	
	ECharacterClass CharacterClass;
	
	virtual void InitAbilityActorInfo();
	
	virtual void InitializeDefaultAttributes() const;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	
	void AddCharacterAbilities();
	
	//溶解效果
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartCharacterDissolveTimeline(UMaterialInstanceDynamic* DynamicDissolveMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicDissolveMaterialInstance);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterialInstance;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<UNiagaraSystem> BloodEffectNiagara;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<USoundBase> DeathSound;
	

	
private:
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};