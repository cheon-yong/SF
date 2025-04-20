// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFInputHandler.generated.h"

class UInputMappingContext;
class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class ASFPlayerController;
class ACharacter;


UENUM(BlueprintType)
enum class EControlType : uint8
{
	ThirdPerson UMETA(DisplayName = "ThirdPerson"),
	SideScroll	UMETA(DisplayName = "SideScroll")
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class SF_API USFInputHandler : public UObject
{
	GENERATED_BODY()

public:
	virtual void AddMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem);
	virtual void RemoveMappingContext();
	virtual void Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent);
	virtual void Unbind();

	void RemoveInputHandler();

	virtual void Tick(float DeltaSeconds) PURE_VIRTUAL(USFInputHandler::Tick, );

protected:
	ACharacter* GetCharacter();
	APawn* GetPawn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;

protected:
	UPROPERTY()
	ASFPlayerController* SFPlayerController;

	UPROPERTY()
	UEnhancedInputComponent* InputComponent;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;
};