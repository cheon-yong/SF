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

UE_DISABLE_OPTIMIZATION
ASFPlayerController::ASFPlayerController()
{
	
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

void ASFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASFPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASFPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASFPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASFPlayerController::Look);

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
