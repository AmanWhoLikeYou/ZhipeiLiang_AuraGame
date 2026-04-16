// Zhipei Liang


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));

}



// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	
}

void AAuraEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor,
	TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
	{
		return;
	}
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	bool bIsInfinite = SpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// 瞬时效果在 BeginOverlap 时应用（若有多个则循环应用）
	if(InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : InstantGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}

	// 持续效果在 BeginOverlap 时应用（若有多个则循环应用）
	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : DurationGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}

	// 无限效果在 BeginOverlap 时应用（若有多个则循环应用）
	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : InfiniteGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 瞬时效果在 EndOverlap 时应用（若有多个则循环应用）
	if (InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : InstantGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}

	// 持续效果在 EndOverlap 时应用（若有多个则循环应用）
	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : DurationGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}

	// 无限效果在 EndOverlap 时应用（若有多个则循环应用）
	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : InfiniteGameplayEffectClasses)
		{
			if (GEClass)
			{
				ApplyGameplayEffectToTarget(TargetActor, GEClass);
			}
		}
	}
	
	if (InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (const auto& Pair : ActiveEffectHandles)
		{
			FActiveGameplayEffectHandle ActiveEffectHandle = Pair.Key;
			UAbilitySystemComponent* NowASC = Pair.Value;
			if (NowASC == nullptr)
			{
				HandlesToRemove.Add(ActiveEffectHandle);
				continue;
			}
			if (NowASC == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle, 1);
				HandlesToRemove.Add(ActiveEffectHandle);
			}
		}
		for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.Remove(Handle);
		}
	}
}
