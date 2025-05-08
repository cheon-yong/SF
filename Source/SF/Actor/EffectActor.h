	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

UCLASS()
class SF_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectActor();

	UFUNCTION(BlueprintImplementableEvent)
	void Burst(AActor* TargetActor);
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
