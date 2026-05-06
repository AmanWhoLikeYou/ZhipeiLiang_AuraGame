// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

struct FScalableFloat;
class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,Category="Class Defalt")
	TSubclassOf<UGameplayEffect> PrimaryAttributesInitializer;
	
	UPROPERTY(EditDefaultsOnly,Category="Class Defalt")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly,Category="Class Defalt")
	FScalableFloat XPReward = FScalableFloat(0);
};
/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Defalt")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassDefaultInfoMap;
	
	UPROPERTY(EditDefaultsOnly,Category="Common Class Defalt")
	TSubclassOf<UGameplayEffect> SecondaryAttributesInitializer;
	UPROPERTY(EditDefaultsOnly,Category="Common Class Defalt")
	TSubclassOf<UGameplayEffect> VitalAttributeInitializer;
	
	
	UPROPERTY(EditDefaultsOnly,Category="Common Class Defalt")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	UPROPERTY(EditDefaultsOnly,Category="Common Class Defalt|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
	
};
