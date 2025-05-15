// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SFInputHandler_AutoRunning.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SFPlayerController.h"
#include "GameFramework/Character.h"
#include <Character/SFPlayerCharacter.h>

void USFInputHandler_AutoRunning::Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::Bind(PlayerController, EnhancedInputComponent);

	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &USFInputHandler_AutoRunning::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &USFInputHandler_AutoRunning::StopJumping);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USFInputHandler_AutoRunning::Move);

	// Dash
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &USFInputHandler_AutoRunning::Dash);

	// Speed
	EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Started, this, &USFInputHandler_AutoRunning::Speed);
	EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Completed, this, &USFInputHandler_AutoRunning::SpeedEnd);
}

void USFInputHandler_AutoRunning::Unbind()
{
	Super::Unbind();
}

void USFInputHandler_AutoRunning::Tick(float DeltaSeconds)
{
	if (bAutoRunning)
	{
		MoveForward();
	}

	//UpdateRespawn();
}

void USFInputHandler_AutoRunning::Move(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// add movement 
		ControlledPawn->AddMovementInput(FVector::RightVector, Direction);
	}
}

void USFInputHandler_AutoRunning::Speed(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (Direction > 0)
	{
		SpeedRate = 1.0f;
	}
	else
	{
		SpeedRate = 0.3f;
	}
}

void USFInputHandler_AutoRunning::SpeedEnd(const FInputActionValue& Value)
{
	SpeedRate = 1.0f;
}

void USFInputHandler_AutoRunning::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void USFInputHandler_AutoRunning::Dash()
{
	if (ASFPlayerCharacter* ControlledCharacter = Cast<ASFPlayerCharacter>(GetCharacter()))
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastDashTime >= DashCoolTime)
		{
			LastDashTime = CurrentTime;
			ControlledCharacter->Dash(DashMontage, DashEffectClass);
		}
	}
}

void USFInputHandler_AutoRunning::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

void USFInputHandler_AutoRunning::MoveForward()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector Direction = FVector::ForwardVector * SpeedRate;
		// add movement 
		ControlledPawn->AddMovementInput(Direction, -1.0f);
	}
}

void USFInputHandler_AutoRunning::UpdateRespawn()
{
	if (ASFPlayerCharacter* ControlledCharacter = Cast<ASFPlayerCharacter>(GetCharacter()))
	{
		ControlledCharacter->UpdateRespawn(RespawnOffset);
	}
}
