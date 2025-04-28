// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/SFInputHandler.h"
#include "SFInputHandler_SideScroll.generated.h"

class UInputAction;
struct FInputActionValue;
class ASFCameraActor_SideScroll;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SF_API USFInputHandler_SideScroll : public USFInputHandler
{
	GENERATED_BODY()
	
public:
	USFInputHandler_SideScroll();

	virtual void Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual void Unbind() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetAimOffset();

	void DrawDebug2DVector(const FVector2D& Vector2D, const FVector& Origin, float Scale = 100.0f, FColor Color = FColor::Green);

protected:
	void Shoot();

	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump();

	void WallJump();

	bool WallInFront();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	UAnimMontage* WallJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float AimAngleUpdateThreshold = 0.2f;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float PrevPitch;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	FRotator TargetRotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator OriginRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASFCameraActor_SideScroll* SideCamera;
};
