// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * 
 * 单例
 */
struct  FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	// Primary Attributes
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	//Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	//Resistance
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	//Meta Attributes
	FGameplayTag Attributes_Meta_IncomingXP;
	
	
	//InputTag
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	
	//SetByCallerTag
	FGameplayTag Damage;
	//伤害类型Tag
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	
	
	//角色状态标签(Effect Granted Tag)
	FGameplayTag Effects_HitReact;
	
	//能力标签
	//SpellGlobeTag(AssetTag Or None)
	FGameplayTag Abilities_None;
	
	//Enemy
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;//被击反应能力标签
	//Aura能力资产标签
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	
	//能力状态标签，标记Aura对能力的使用情况
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	//标记能力是主动还是被动还是辅助能力
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	

	
	//标记武器插槽标签
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	
	//蒙太奇标记标签
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	//Cooldown标签
	FGameplayTag Cooldown_Fire_FireBolt;
	
private:
	static FAuraGameplayTags GameplayTags;
};
