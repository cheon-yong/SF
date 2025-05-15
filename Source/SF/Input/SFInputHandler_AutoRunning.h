// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/SFInputHandler.h"
#include "SFInputHandler_AutoRunning.generated.h"

class UInputAction;
struct FInputActionValue;
class AEffectActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SF_API USFInputHandler_AutoRunning : public USFInputHandler
{
	GENERATED_BODY()
	
public:

	virtual void Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual void Unbind() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	void Move(const FInputActionValue& Value);

	void Speed(const FInputActionValue& Value);
	void SpeedEnd(const FInputActionValue& Value);

	void Jump();

	void Dash();

	void StopJumping();

	void MoveForward();

	void UpdateRespawn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash)
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash)
	TSubclassOf<AEffectActor> DashEffectClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash)
	float DashCoolTime = 2.f;

	UPROPERTY(Transient)
	float LastDashTime = -FLT_MAX;

	UPROPERTY(Transient)
	float SpeedRate = 0.7f;

	UPROPERTY(Transient)
	FVector RespawnOffset = FVector(-500, 0, 0);

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpeedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoRunning = false;
};
