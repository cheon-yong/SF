// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrapActor.generated.h"

class ASFCharacter;

UCLASS()
class SF_API ATrapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapActor();

protected:

	UFUNCTION(BlueprintCallable)
	virtual void GiveDamage(ASFCharacter* SFCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float Damage = 0.f;
};
