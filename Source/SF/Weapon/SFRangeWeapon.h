// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/SFWeapon.h"
#include "SFRangeWeapon.generated.h"

class ASFProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFire);

/**
 * 
 */
UCLASS()
class SF_API ASFRangeWeapon : public ASFWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Attack() override;

	UPROPERTY(BlueprintAssignable)
	FOnFire OnFire;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bullet)
	TSubclassOf<ASFProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bullet)
	FName SpawnSocketName;
};
