// Zhipei Liang


#include "Player/AuraPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToLevel(int32 AddLevelAmount)
{
	Level += AddLevelAmount;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetLevel(int32 InLevelAmount)
{
	Level = InLevelAmount;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToXP(int32 AddXPAmount)
{
	XP += AddXPAmount;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetXP(int32 InXPAmount)
{
	XP = InXPAmount;
	OnXPChangedDelegate.Broadcast(XP);
}


void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
