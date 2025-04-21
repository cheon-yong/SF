// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SFTeleportActor.generated.h"

class ACharacter;

UCLASS()
class SF_API ASFTeleportActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFTeleportActor();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_TeleportActors(const TArray<ACharacter*>& Characters);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	TArray<AActor*> TeleportPoints;
};
