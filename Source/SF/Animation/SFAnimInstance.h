// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SFAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SF_API USFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USFAnimInstance> TargetAnim;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling;
};
