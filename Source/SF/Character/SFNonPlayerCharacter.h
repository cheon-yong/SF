// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SFNonPlayerCharacter.generated.h"

UCLASS()
class SF_API ASFNonPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASFNonPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	float GetAIPatrolRadius();
	float GetAITurnSpeed();
	float GetAIAttackRange();
	float GetAIDetectRadius();

public:
	bool bDebug = false;

protected: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float TurnSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectRadius = 200.f;
};
