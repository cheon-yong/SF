// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UMaterialInstance;
class UAnimationAsset;
class UAnimInstance;
class ASFWeapon;
class USFWeaponData;
class USFUserWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHpChanged, const uint8&, MaxHp, const uint8&, CurrentHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpZero);


UENUM(BlueprintType)
enum class ECharacterColor : uint8
{
	Blue = 0,
	Red = 1
};

USTRUCT(BlueprintType)
struct FCharacterMaterials
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> Material0;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> Material1;
};


UCLASS(config=Game)
class ASFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASFCharacter();
			
	// Weapon
	void AddWeapon(TSubclassOf<USFWeaponData> WeaponDataClass);
	void RemoveWeapon(TObjectPtr<USFWeaponData> WeaponData);
	void EquipWeapon(TObjectPtr<USFWeaponData> WeaponData);
	void UnequipWeapon(TObjectPtr<USFWeaponData> WeaponData);

	virtual void UseWeapon();
	// ~Weapon

	virtual void OnDamage(uint8 Damage, AActor* instigator);

	UFUNCTION()
	virtual void OnDeath();

	virtual void SetupCharacterWidget(USFUserWidget* InUserWidget);

	UPROPERTY(BlueprintAssignable)
	FOnHpChanged OnHpChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHpZero OnHpZero;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 MaxHp = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 CurrentHp = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<TObjectPtr<USFWeaponData>> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<USFWeaponData> CurrentWeapon;
};

