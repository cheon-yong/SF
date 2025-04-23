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

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;

	void SetColor();



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch_SideScroll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToMouseVector;

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color, meta = (AllowPrivateAccess = "true"))
	TArray<FCharacterMaterials> CharacterColorMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<TObjectPtr<USFWeaponData>> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<USFWeaponData> CurrentWeapon;
};

