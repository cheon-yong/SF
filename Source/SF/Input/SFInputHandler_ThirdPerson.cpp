// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SFInputHandler_ThirdPerson.h"

#include "Actor/InteractActor.h"
#include "Character/SFPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SFPlayerController.h"
#include "GameFramework/Character.h"

void USFInputHandler_ThirdPerson::Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::Bind(PlayerController, EnhancedInputComponent);
	
	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &USFInputHandler_ThirdPerson::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &USFInputHandler_ThirdPerson::StopJumping);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USFInputHandler_ThirdPerson::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &USFInputHandler_ThirdPerson::Look);

	// Interact
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &USFInputHandler_ThirdPerson::Interact);
}

void USFInputHandler_ThirdPerson::Tick(float DeltaSeconds)
{
	
}

void USFInputHandler_ThirdPerson::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = SFPlayerController->GetControlRotation();
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

void USFInputHandler_ThirdPerson::Look(const FInputActionValue& Value)
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

void USFInputHandler_ThirdPerson::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void USFInputHandler_ThirdPerson::Interact()
{
	if (ASFPlayerCharacter* SFPlayerCharacter = Cast<ASFPlayerCharacter>(GetCharacter()))
	{
		TArray<AActor*> Actors = SFPlayerCharacter->GetInteractActors();

		AInteractActor* PrimaryActor = nullptr;
		float MinDist = FLT_MAX;
		for (AActor* Actor : Actors)
		{
			if (AInteractActor* IA = Cast<AInteractActor>(Actor))
			{
				float Dist = IA->GetDistanceTo(SFPlayerCharacter);
				if (Dist < MinDist)
				{
					PrimaryActor = IA;
					MinDist = Dist;
				}
			}
		}

		if (PrimaryActor == nullptr)
		{
			return;
		}

		PrimaryActor->BeginInteract(SFPlayerCharacter);
	}
}

void USFInputHandler_ThirdPerson::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}
