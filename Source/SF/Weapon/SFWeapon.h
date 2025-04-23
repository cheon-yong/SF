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

	virtual void Attack();

	virtual void OnEquipped();
	
	virtual void OnUnequipped();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> MeshComp;
};
