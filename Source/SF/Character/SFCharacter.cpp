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
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// FInventory

void FInventory::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FInventoryEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			if (Entry.Instance->SpawnedWeapon)
				Entry.Instance->SpawnedWeapon->OnUnequipped();

			Entry.Instance->SpawnedWeapon = nullptr;
		}
	}
}

void FInventory::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FInventory::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

USFWeaponData* FInventory::AddEnty(TSubclassOf<USFWeaponData> InWeaponDataClass)
{
	USFWeaponData* Result = nullptr;

	check(InWeaponDataClass != nullptr);
	check(Owner);
	check(Owner->HasAuthority());

	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.WeaponDataClass = InWeaponDataClass;
	NewEntry.Instance = NewObject<USFWeaponData>(Owner, InWeaponDataClass);
	Result = NewEntry.Instance;

	MarkItemDirty(NewEntry);

	return Result;
}

void FInventory::RemoveEntry(USFWeaponData* InInstance)
{
}


//////////////////////////////////////////////////////////////////////////
// ASFCharacter

ASFCharacter::ASFCharacter()
	: Super()
	, Inventory(this)
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

void ASFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Inventory);
}


void ASFCharacter::AddWeapon(TSubclassOf<USFWeaponData> WeaponDataClass)
{
	Inventory.AddEnty(WeaponDataClass);

	if (CurrentWeapon == nullptr)
	{
		EquipWeapon(0);
	}
}

void ASFCharacter::RemoveWeapon(TObjectPtr<USFWeaponData> WeaponData)
{
}

void ASFCharacter::EquipWeapon(int32 Index)
{
	if (HasAuthority())
	{
		if(CurrentWeapon != nullptr)
		{
			UnequipWeapon();
		}

		CurrentWeapon = Inventory.Entries[Index].Instance;
		SpawnWeapon();
	}
}

void ASFCharacter::SpawnWeapon()
{
	CurrentWeapon->SpawnWeapon(this);
	if (CurrentWeapon->AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(CurrentWeapon->AnimInstanceClass);
	}
}

void ASFCharacter::UnequipWeapon()
{
	//Inventory.UnequipWeapon(Index);
	if (CurrentWeapon->SpawnedWeapon)
	{
		CurrentWeapon->SpawnedWeapon->OnUnequipped();
		CurrentWeapon->SpawnedWeapon->Destroy();
		CurrentWeapon->SpawnedWeapon = nullptr;
	}

	CurrentWeapon = nullptr;
}

void ASFCharacter::UseWeapon()
{
	if (CurrentWeapon != nullptr)
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

void ASFCharacter::OnRep_Weapons()
{
	SpawnWeapon();
}

void ASFCharacter::OnRep_WeaponIndex()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
		FString::Printf(TEXT("OnRep_WeaponIndex")));
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

