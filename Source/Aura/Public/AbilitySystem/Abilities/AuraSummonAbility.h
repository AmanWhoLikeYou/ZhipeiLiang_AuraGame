// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations() const;
	
	UFUNCTION(BlueprintPure)
	TSubclassOf<APawn> GetRandomMinionClass() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 MinionsNum = 5;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 MinSpawnDistance = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 MaxSpawnDistance = 250.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float SpawnSpread = 90.f;
	
};
