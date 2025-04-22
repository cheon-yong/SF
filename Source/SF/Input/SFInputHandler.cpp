// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SFInputHandler.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/SFPlayerController.h"

void USFInputHandler::AddMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem)
{
	InputSubsystem = Subsystem;

	InputSubsystem->AddMappingContext(MappingContext, 0);
}

void USFInputHandler::RemoveMappingContext()
{
	if (InputSubsystem)
		InputSubsystem->RemoveMappingContext(MappingContext);
}

void USFInputHandler::Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent)
{
	SFPlayerController = PlayerController;
	InputComponent = EnhancedInputComponent;
}

void USFInputHandler::Unbind()
{
	if (InputComponent)
		InputComponent->ClearActionBindings();

	SFPlayerController = nullptr;
	InputComponent = nullptr;
	InputSubsystem = nullptr;
}

void USFInputHandler::RemoveInputHandler()
{
	RemoveMappingContext();
	Unbind();
}

ACharacter* USFInputHandler::GetCharacter()
{
	return SFPlayerController ? SFPlayerController->GetCharacter() : nullptr;
}

APawn* USFInputHandler::GetPawn()
{
	 return SFPlayerController ? SFPlayerController->GetPawn() : nullptr; 
}
