// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFNonPlayerCharacter.h"

// Sets default values
ASFNonPlayerCharacter::ASFNonPlayerCharacter()
{
}

// Called when the game starts or when spawned
void ASFNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ASFNonPlayerCharacter::GetAIPatrolRadius()
{
	return PatrolRadius;
}

float ASFNonPlayerCharacter::GetAITurnSpeed()
{
	return TurnSpeed;
}

float ASFNonPlayerCharacter::GetAIAttackRange()
{
	return AttackRange;
}

float ASFNonPlayerCharacter::GetAIDetectRadius()
{
	return DetectRadius;
}

