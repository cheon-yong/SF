// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

UCLASS()
class AInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginInteract(AActor* InInteractPawn);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EndInteract(AActor* InInteractPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
