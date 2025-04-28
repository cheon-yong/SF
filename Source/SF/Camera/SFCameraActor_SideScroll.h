// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SFCameraActor_SideScroll.generated.h"

class USpringArmComponent;
/**
 * 
 */
UCLASS()
class SF_API ASFCameraActor_SideScroll : public ACameraActor
{
	GENERATED_BODY()
	
public:
	ASFCameraActor_SideScroll();

	UFUNCTION(BlueprintCallable)
	void SetActiveCamera(bool Active);

	UFUNCTION(BlueprintCallable)
	void GetPlayers();

protected:
	virtual void Tick(float DeltaTime) override;

	void GetTargetValues();

	FVector CalculateAverageLocation();
	float CalculateArmLength();
	float GetPlayersDistance();
	void SetInterpolationValue(float DeltaTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ACharacter>> Players;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MinArmLength = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MaxArmLength = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bActivating;
};
