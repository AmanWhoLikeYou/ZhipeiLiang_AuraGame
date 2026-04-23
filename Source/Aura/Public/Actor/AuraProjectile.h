// Zhipei Liang

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

struct FGameplayEffectSpecHandle;
class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	
	
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedDataAsset, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> HitNiagaraSystem;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HitSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FlySound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> FlyAudioComponent;
	
	bool bHit = false;
};
