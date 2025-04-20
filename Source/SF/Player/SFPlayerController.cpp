// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFPlayerController.h"
#include "Game/SFGameMode.h"
#include "Game/SFGameInstance.h"
#include "Player/SFPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/SFCharacter.h"
#include "Player/SFLocalPlayer.h"
#include "Camera/SFGameViewportClient.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>


ASFPlayerController::ASFPlayerController()
{
	DefaultType = EControlType::ThirdPerson;
}


void ASFPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			if (GameInstance->GetNumLocalPlayers() < 2)
			{
				FString Error;
				int32 NewId = GameInstance->GetLocalPlayerByIndex(0)->GetControllerId() + 1;
				GameInstance->CreateLocalPlayer(NewId, Error, true);
			}
			else
			{
				Server_DeleteSecondPawn();
			}
		}
	}
}

void ASFPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void ASFPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentInputHandler)
	{
		CurrentInputHandler->Tick(DeltaSeconds);
	}
}

void ASFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TSubclassOf<USFInputHandler> InputHandlerClass = InputHandlerClassMap[DefaultType];
	CurrentInputHandler = NewObject<USFInputHandler>(this, InputHandlerClass);

	if (CurrentInputHandler == nullptr)
	{
		return;
	}

	BindInputHandler(CurrentInputHandler);

}

UE_DISABLE_OPTIMIZATION
void ASFPlayerController::ChangeControlType(EControlType NewControlType)
{
	TSubclassOf<USFInputHandler> NewInputHandlerClass = InputHandlerClassMap[NewControlType];
	if (CurrentInputHandler->StaticClass() == NewInputHandlerClass)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
		FString::Printf(TEXT("Change ControlType")));

	CurrentInputHandler->RemoveInputHandler();

	USFInputHandler* NewInputHandler = NewObject<USFInputHandler>(this, NewInputHandlerClass);
	BindInputHandler(NewInputHandler);
}
UE_ENABLE_OPTIMIZATION


void ASFPlayerController::BindInputHandler(USFInputHandler* InputHandler)
{

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputHandler->AddMappingContext(Subsystem);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		InputHandler->Bind(this, EnhancedInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASFPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASFPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ASFPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void ASFPlayerController::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}



void ASFPlayerController::Client_UpdateSecondController_Implementation()
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		auto LocalPlayers = GameInstance->GetLocalPlayers();
		for (const ULocalPlayer* LC : LocalPlayers)
		{
			if (APlayerController* PC = LC->GetPlayerController(GetWorld()))
			{
				if (PC->GetPawn())
					continue;	

				PC->bAutoManageActiveCameraTarget = false;	
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
				FString::Printf(TEXT("Controller : %s"), *LC->GetPlayerController(GetWorld())->GetViewTarget()->GetName()));

		}

		if (USFGameViewportClient* SFGameViewportClient = Cast<USFGameViewportClient>(GameInstance->GetGameViewportClient()))
		{
			SFGameViewportClient->SwapCameraPosition();
		}
	}
}


void ASFPlayerController::Server_DeleteSecondPawn_Implementation()
{
	AActor* NewViewTarget = nullptr;
	for (FConstPlayerControllerIterator PCIt = GetWorld()->GetPlayerControllerIterator(); PCIt; ++PCIt)
	{
		APlayerController* PC = PCIt->Get();
		if (PC->IsLocalController())
		{
			NewViewTarget = PC->GetViewTarget();
			continue;
		}
		
		if (PCIt.GetIndex() == GetWorld()->GetNumPlayerControllers() - 1)
		{
			//PCIt->Get()->UnPossess();
			APawn* NewPawn = PC->GetPawn();
			PC->UnPossess();
			if (NewPawn)
				GetWorld()->DestroyActor(NewPawn);

			PC->SetViewTarget(NewViewTarget);
			
			Client_UpdateSecondController();
		}
	}
}
