// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SFAnimInstance_Copy.h"

void USFAnimInstance_Copy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (TargetAnim)
	{
		Velocity = TargetAnim->Velocity;
		GroundSpeed = TargetAnim->GroundSpeed;
		bShouldMove = TargetAnim->bShouldMove;
		bIsFalling = TargetAnim->bIsFalling;
	}
}