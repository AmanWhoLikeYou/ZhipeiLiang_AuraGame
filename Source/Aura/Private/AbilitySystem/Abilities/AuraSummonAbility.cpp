// Zhipei Liang


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	
	const FVector Forward = AvatarActor->GetActorForwardVector();
	const FVector Location = AvatarActor->GetActorLocation();
	
	TArray<FVector> SpawnLocations;
	
	float DeltaSpread = SpawnSpread / MinionsNum;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	for (int32 i = 0; i < MinionsNum; ++i)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector SpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation + FVector::UpVector * 500.f, SpawnLocation - FVector::UpVector * 500.f, ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			SpawnLocation = HitResult.ImpactPoint;
		}
		
		SpawnLocations.Add(SpawnLocation);
		
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Location, SpawnLocation, 10.f, FLinearColor::Red, 5.f);
	}
	
	return SpawnLocations;
	
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass() const
{
	if (MinionClasses.Num() == 0)
	{
		return nullptr;
	}
	
	const int32 RandomIndex = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[RandomIndex];
}
