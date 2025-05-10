// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/SFInputHandler.h"
#include "SFInputHandler_ThirdPerson.generated.h"

class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SF_API USFInputHandler_ThirdPerson : public USFInputHandler
{
	GENERATED_BODY()
	
public:

	virtual void Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump();

	void Interact();

	void StopJumping();

public:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

};
