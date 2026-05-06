// Zhipei Liang


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags= FAuraGameplayTags::Get();;
	
	//Primary Attribute
	TagsToAttribute.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	
	//Secondary Attribute
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
	//ResistanceAttributes
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	
	
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	//Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	//Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	//Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// 获取当前世界的网络模式（用于区分是客户端还是服务器）并在日志中打印
	const TCHAR* NetModeString = TEXT("Unknown");
	if (GetWorld())
	{
		ENetMode Mode = GetWorld()->GetNetMode();
		if (Mode == NM_Client)
		{
			NetModeString = TEXT("Client");
		}
		else if (Mode == NM_DedicatedServer)
		{
			NetModeString = TEXT("DedicatedServer");
		}
		else if (Mode == NM_Standalone)
		{
			NetModeString = TEXT("Standalone");
		}
		else if (Mode == NM_ListenServer)
		{
			NetModeString = TEXT("ListenServer");
		}
		else
		{
			NetModeString = TEXT("Other");
		}
	}

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		UE_LOG(LogTemp, Log, TEXT("PreAttributeChange [%s]: Health changing to %f"), NetModeString, NewValue);
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("PreAttributeChange [%s]: MaxHealth changing to %f"), NetModeString, NewValue);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		UE_LOG(LogTemp, Log, TEXT("PreAttributeChange [%s]: Mana changing to %f"), NetModeString, NewValue);
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("PreAttributeChange [%s]: MaxMana changing to %f"), NetModeString, NewValue);
	}

}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties) const
{
		
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	EffectProperties.EffectContextHandle = Context;

	UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();
	EffectProperties.SourceASC = SourceASC;
	
	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
		AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
		EffectProperties.SourceAvatarActor = SourceAvatarActor;
		
		if (SourceController == nullptr && SourceAvatarActor != nullptr)
		{
			if (const APawn* SourcePawn = Cast<APawn>(SourceAvatarActor))
			{
				SourceController = SourcePawn->GetController();
			}
		}
		if (SourceController)
		{
			ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			EffectProperties.SourceCharacter = SourceCharacter;
		}
		
		EffectProperties.SourceController = SourceController;
		
	}
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		if (TargetController == nullptr && TargetAvatarActor != nullptr)
		{
			if (const APawn* TargetPawn = Cast<APawn>(TargetAvatarActor))
			{
				TargetController = TargetPawn->GetController();
			}
		}
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
		EffectProperties.TargetAvatarActor = TargetAvatarActor;
		EffectProperties.TargetController = TargetController;
		EffectProperties.TargetCharacter = TargetCharacter;
		EffectProperties.TargetASC = TargetASC;
	}
}



void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Log, TEXT("PostGameplayEffectExecute: On %s Health changed to %f"), *EffectProperties.TargetAvatarActor->GetName() , GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth =GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
				SendXPEvent(EffectProperties);
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		
			bool bBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
			ShowFloatingDamageText(EffectProperties,LocalIncomingDamage,bBlockedHit,bCriticalHit);
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);
		
		//SourceCharacter is Aura
		if (EffectProperties.SourceCharacter->Implements<UPlayerInterface>() && EffectProperties.SourceCharacter->Implements<UCombatInterface>())
		{
			int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.SourceCharacter);
			int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);
			
			int32 NextLevel = IPlayerInterface::Execute_GetLevelFromXP(EffectProperties.SourceCharacter, CurrentXP + LocalIncomingXP);
			int32 LevelUpNum = NextLevel - CurrentLevel;
			
			if (LevelUpNum > 0)
			{
				int32 AttributePointReward = 0;
				int32 SpellPointReward = 0;
				for (int Level = CurrentLevel; Level < NextLevel; Level++)
				{
					AttributePointReward += IPlayerInterface::Execute_GetAttributePointReward(EffectProperties.SourceCharacter, Level);
					SpellPointReward += IPlayerInterface::Execute_GetSpellPointReward(EffectProperties.SourceCharacter, Level);
				}
				IPlayerInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter, AttributePointReward);
				IPlayerInterface::Execute_AddToSpellPoints(EffectProperties.SourceCharacter, SpellPointReward);
				IPlayerInterface::Execute_AddToPlayerLevel(EffectProperties.SourceCharacter, LevelUpNum);
				
				SetHealth(GetMaxHealth());
				SetMana(GetMaxMana());
				
				IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
				
			}
			
			IPlayerInterface::Execute_AddXP(EffectProperties.SourceCharacter, LocalIncomingXP);
		}
	}
	
}

void UAuraAttributeSet::ShowFloatingDamageText(FEffectProperties& EffectProperties, float DamageAmount, bool bBlockedHit, bool bCriticalHit) const
{
	if (!IsValid(EffectProperties.TargetCharacter)) return;

	AAuraPlayerController* SourcePC = Cast<AAuraPlayerController>(EffectProperties.SourceController);
	AAuraPlayerController* TargetPC = Cast<AAuraPlayerController>(EffectProperties.TargetController);

	if (SourcePC)
	{
		SourcePC->ShowDamageNumber(DamageAmount, EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
	}

	if (TargetPC && TargetPC != SourcePC)
	{
		TargetPC->ShowDamageNumber(DamageAmount, EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& EffectProperties) const
{
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(EffectProperties.TargetCharacter);
		int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardByClassAndLevel(EffectProperties.TargetCharacter, TargetClass, TargetLevel);
		
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData EventData;
		EventData.EventTag = GameplayTags.Attributes_Meta_IncomingXP; 
		EventData.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, EventData);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}


void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}


void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}


void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}
