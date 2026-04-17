// Zhipei Liang


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(30.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
		EffectProperties.TargetAvatarActor = SourceAvatarActor;
		
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

	
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
