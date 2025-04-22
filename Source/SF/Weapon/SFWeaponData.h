// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SFWeaponData.generated.h"

class ASFWeapon;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SF_API USFWeaponData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASFWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ASFWeapon> SpawnedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;
	
};
