	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedBurst);

UCLASS()
class SF_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectActor();

	UFUNCTION(BlueprintImplementableEvent)
	void Burst(AActor* TargetActor);
		
	UPROPERTY(BlueprintAssignable)
	FOnFinishedBurst OnFinishedBurst;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
