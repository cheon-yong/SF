// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SFWeaponData.generated.h"

class ASFWeapon;
class ASFCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SF_API USFWeaponData : public UObject
{
	GENERATED_BODY()
	
public:

	virtual bool IsSupportedForNetworking() const override { return true; }

	void SetOwner(AActor* InOwner);

	void SetInstigator(APawn* InInstigator);
	
	ASFWeapon* SpawnWeapon(ASFCharacter* InOwner);

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
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APawn* Instigator;
};
