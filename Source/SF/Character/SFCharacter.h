// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Net/Serialization/FastArraySerializer.h"
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

USTRUCT(BlueprintType)
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry() {}

	friend FInventory;

public:
	UPROPERTY()
	TSubclassOf<USFWeaponData> WeaponDataClass;

	UPROPERTY()
	TObjectPtr<USFWeaponData> Instance;
};

USTRUCT(BlueprintType)
struct FInventory : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FInventory() {}

	FInventory(AActor* InOwner)
		: Owner(InOwner)
	{

	}

public:

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	USFWeaponData* AddEnty(TSubclassOf<USFWeaponData> InWeaponDataClass);
	void RemoveEntry(USFWeaponData* InInstance);

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<AActor> Owner;


	friend ASFCharacter;
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

	void EquipWeapon(int32 Index);

	void SpawnWeapon();

	void UnequipWeapon();

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
	UFUNCTION()
	void OnRep_Weapons();

	UFUNCTION()
	void OnRep_WeaponIndex();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 MaxHp = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 CurrentHp = 20;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	FInventory Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<USFWeaponData> CurrentWeapon = nullptr;
};

