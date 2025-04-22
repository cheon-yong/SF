// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SFWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SF_API ASFWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFWeapon();

	virtual void Shoot();

	virtual void OnEquipped();
	
	virtual void OnUnequipped();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> MeshComp;
};
