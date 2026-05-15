// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class UAuraWidgetController;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuAWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutHUD);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuAWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC ,float Level);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass );
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutActors, const FVector& SphereOrigin, const float Radius, const TArray<AActor*>& ActorsToIgnore);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriendly(const AActor* ActorA, const AActor* ActorB);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static int32 GetXPRewardByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
