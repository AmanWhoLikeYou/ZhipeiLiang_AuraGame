// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT()
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirementXP = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInfos;
	
	int32 FindLevelFromXP(int32 XP) const;
};
