// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponGiver.generated.h"

class ASFCharacter;
class USFWeaponData;

UCLASS(Blueprintable, BlueprintType)
class SF_API AWeaponGiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponGiver();

	UFUNCTION(BlueprintCallable)
	void GiveWeapon(ASFCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USFWeaponData>> WeaponClass;
};
