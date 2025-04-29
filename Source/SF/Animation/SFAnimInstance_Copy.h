// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SFAnimInstance.h"
#include "SFAnimInstance_Copy.generated.h"

/**
 * 
 */
UCLASS()
class SF_API USFAnimInstance_Copy : public USFAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
