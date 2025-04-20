// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Character.h"
#include "Player/SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Input/SFInputHandler_SideScroll.h"

void USFInputHandler_SideScroll::Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::Bind(PlayerController, EnhancedInputComponent);

	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	OriginRate = GetCharacter()->GetCharacterMovement()->RotationRate;

	GetCharacter()->GetCharacterMovement()->RotationRate = TargetRotationRate;
}

void USFInputHandler_SideScroll::Unbind()
{
	GetCharacter()->GetCharacterMovement()->RotationRate = OriginRate;

	Super::Unbind();
}

void USFInputHandler_SideScroll::Tick(float DeltaSeconds)
{
}

void USFInputHandler_SideScroll::Move(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// add movement 
		

		// 횡스크롤에서는 일반적으로 X축 또는 Y축 한쪽으로만 이동
		// 예: X축 방향이 오른쪽이라면
		ControlledPawn->AddMovementInput(FVector::BackwardVector, Direction);
	}
}

void USFInputHandler_SideScroll::Look(const FInputActionValue& Value)
{
}

void USFInputHandler_SideScroll::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void USFInputHandler_SideScroll::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}