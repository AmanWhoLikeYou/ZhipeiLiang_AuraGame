// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent)
	void AddXP(int32 AddXPAmount);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevelFromXP(int32 XP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointReward(int32 Level)const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointReward(int32 Level)const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 AddLevelAmount);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 AddAttributePointAmount);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 AddSpellPointAmount);
	
	
};
