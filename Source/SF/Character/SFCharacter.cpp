// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include <Game/SFGameMode.h>
#include "Weapon/SFWeapon.h"
#include "Weapon/SFWeaponData.h"
#include "Widget/HpBar.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASFCharacter

ASFCharacter::ASFCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	
}

void ASFCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	OnHpZero.AddDynamic(this, &ThisClass::OnDeath);
}


void ASFCharacter::AddWeapon(TSubclassOf<USFWeaponData> WeaponDataClass)
{
	USFWeaponData* NewWeaponData = NewObject<USFWeaponData>(this, WeaponDataClass);

	NewWeaponData->SetOwner(this);
	NewWeaponData->SetInstigator(this);
	Weapons.Add(NewWeaponData);

	if (CurrentWeapon == nullptr)
	{
		EquipWeapon(NewWeaponData);
	}
}

void ASFCharacter::RemoveWeapon(TObjectPtr<USFWeaponData> WeaponData)
{
}

void ASFCharacter::EquipWeapon(TObjectPtr<USFWeaponData> WeaponData)
{
	if (CurrentWeapon != nullptr)
	{
		UnequipWeapon(CurrentWeapon);
	}
	
	WeaponData->SpawnWeapon(this);
	CurrentWeapon = WeaponData;
	if (WeaponData->AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(WeaponData->AnimInstanceClass);
	}
}

void ASFCharacter::UnequipWeapon(TObjectPtr<USFWeaponData> WeaponData)
{
	if (WeaponData->SpawnedWeapon)
	{
		WeaponData->SpawnedWeapon->OnUnequipped();
		WeaponData->SpawnedWeapon->Destroy();
		WeaponData->SpawnedWeapon = nullptr;
	}
}

void ASFCharacter::UseWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SpawnedWeapon->Attack();
	}
}

void ASFCharacter::OnDamage(uint8 Damage, AActor* instigator)
{
	uint8 BeforeHP = CurrentHp;
	CurrentHp = FMath::Clamp(BeforeHP - Damage, 0, MaxHp);

	if (BeforeHP == CurrentHp)
		return;

	OnHpChanged.Broadcast(CurrentHp, MaxHp);

	if (CurrentHp == 0)
	{
		OnHpZero.Broadcast();
	}
}

void ASFCharacter::OnDeath()
{
	Destroy();
}


void ASFCharacter::SetupCharacterWidget(USFUserWidget* InUserWidget)
{
	UHpBar* HpBarWidget = Cast<UHpBar>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHp(CurrentHp, MaxHp);
		OnHpChanged.AddDynamic(HpBarWidget, &UHpBar::UpdateHp);
	}
}