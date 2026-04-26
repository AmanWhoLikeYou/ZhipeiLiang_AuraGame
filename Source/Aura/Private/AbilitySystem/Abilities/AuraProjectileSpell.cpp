// Zhipei Liang


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Character/AuraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Public/AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
}


void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTagetLocation)
{
	
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);

		FRotator Rotation = (ProjectileTagetLocation - SpawnLocation).Rotation();
		Rotation.Pitch = 0.f;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile =  GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTagetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ProjectileDamageEffect, GetAbilityLevel(), EffectContextHandle);
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		
		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Pair.Key, ScaledDamage);
		}

		
		Projectile->DamageEffectSpecHandle = EffectSpecHandle;
		Projectile->Owner = GetAvatarActorFromActorInfo();
		Projectile->FinishSpawning(SpawnTransform);
		
	}
}