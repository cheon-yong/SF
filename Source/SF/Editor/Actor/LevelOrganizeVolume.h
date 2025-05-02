// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelOrganizeVolume.generated.h"

class UBoxComponent;

UCLASS()
class SF_API ALevelOrganizeVolume : public AActor
{
	GENERATED_BODY()
	
public:
    ALevelOrganizeVolume();

#if WITH_EDITOR
    UFUNCTION(CallInEditor, Category = "Editor Tools")
    void MoveOverlappingActorsToFolder();
#endif

protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, Category = "Editor Tools")
    FString TargetFolder = "OrganizedActors";

};
