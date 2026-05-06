// Zhipei Liang


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuAWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			
			return Cast<UOverlayWidgetController>(HUD->GetOverlayWidgetController(WCParams));
		}
	}
	return nullptr;
}

UAttributeMenuAWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			
			return Cast<UAttributeMenuAWidgetController>(HUD->GetAttributeMenuWidgetController(WCParams));
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, UAbilitySystemComponent* ASC, float Level)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributesInitializer,
		Level,PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeHandle.Data.Get());
	
		
	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributesInitializer,
		Level,SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeHandle.Data.Get());
	
		
	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributeInitializer,
		Level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> StartupAbilityOfClass : ClassDefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbilityOfClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* GameMod = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMod == nullptr)return nullptr;
	
	return GameMod->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutActors,
	const FVector& SphereOrigin, const float Radius, const TArray<AActor*>& ActorsToIgnore)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByObjectType(OverlapResults, SphereOrigin, FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), QueryParams);
		
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			const bool bIsImplementingCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (!bIsImplementingCombatInterface) continue;
			const bool bIsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
			if (bIsImplementingCombatInterface && bIsAlive)
			{
				OutActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriendly(const AActor* ActorA, const AActor* ActorB)
{
	const bool AIsPlayer = ActorA->ActorHasTag(FName("Player"));
	const bool BIsPlayer = ActorB->ActorHasTag(FName("Player"));
	
	const bool AIsEnemy = ActorA->ActorHasTag(FName("Enemy"));
	const bool BIsEnemy = ActorB->ActorHasTag(FName("Enemy"));
	
	return (AIsPlayer && BIsEnemy) || (AIsEnemy && BIsPlayer);
}

int32 UAuraAbilitySystemLibrary::GetXPRewardByClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0.f;
	
	const FCharacterClassDefaultInfo* Info = CharacterClassInfo->CharacterClassDefaultInfoMap.Find(CharacterClass);
	const float XPReward = Info->XPReward.GetValueAtLevel(CharacterLevel);
	
	return static_cast<int32>(XPReward);
	
}
