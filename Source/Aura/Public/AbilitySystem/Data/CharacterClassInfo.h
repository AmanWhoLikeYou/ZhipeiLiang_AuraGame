// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

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
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
};
