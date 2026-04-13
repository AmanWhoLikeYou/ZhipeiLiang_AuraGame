
#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Make this controller replicate by default
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem *LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	LocalPlayerSubsystem->AddMappingContext(AuraContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}
