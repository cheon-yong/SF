// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameStateBase.h"
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

UE_DISABLE_OPTIMIZATION
void FInventory::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		if (Entries.IsValidIndex(Index))
		{
			FInventoryEntry& Entry = Entries[Index];
			if (!Entry.Instance && Entry.WeaponDataClass)
			{
				Entry.Instance = NewObject<USFWeaponData>(Owner, Entry.WeaponDataClass);
				UE_LOG(LogTemp, Warning, TEXT("Instance created for index %d"), Index);
			}
		}
	}
}
UE_ENABLE_OPTIMIZATION

void FInventory::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		if (Stack.Instance == nullptr)
		{
			Stack.Instance = NewObject<USFWeaponData>(Owner, Stack.WeaponDataClass);
		}
	}
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
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == InInstance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
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

	SetReplicates(true);
}

void ASFCharacter::OnRep_WeaponIndex()
{
	//SetAnimation();

}

void ASFCharacter::OnRep_CurrentWeapon()
{
	
	SetAnimation();
}

void ASFCharacter::OnRep_MaxHp()
{
}

void ASFCharacter::OnRep_Hp()
{
}

void ASFCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetMesh()->SetAnimInstanceClass(DefaultAnimationClass);

	OnHpZero.AddDynamic(this, &ThisClass::OnDeath);
}

void ASFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Inventory);
	DOREPLIFETIME(ThisClass, WeaponIndex);
	DOREPLIFETIME(ThisClass, CurrentWeapon);
	DOREPLIFETIME(ThisClass, MaxHp);
	DOREPLIFETIME(ThisClass, CurrentHp);
}


void ASFCharacter::AddWeapon(TSubclassOf<USFWeaponData> WeaponDataClass)
{
	if (HasAuthority())
	{
		Inventory.AddEnty(WeaponDataClass);
	
		if (WeaponIndex == INDEX_NONE)
		{
			EquipWeapon(0);
		}
	}
}

void ASFCharacter::RemoveWeapon(TObjectPtr<USFWeaponData> WeaponData)
{
}

void ASFCharacter::EquipWeapon(int32 Index)
{
	if (WeaponIndex != INDEX_NONE)
	{
		UnequipWeapon();
	}

	WeaponIndex = Index;
	if (HasAuthority())
	{
		//SpawnWeapon();
		SpawnWeapon();
	}
}

void ASFCharacter::SpawnWeapon()
{
	CurrentWeapon = Inventory.Entries[WeaponIndex].Instance->SpawnWeapon(this);
	if (HasAuthority())
	{
		SetAnimation();
	}
}

void ASFCharacter::SetAnimation()
{
	if (WeaponIndex == INDEX_NONE)
		return;

	if (USFWeaponData* WeaponCDO = Inventory.Entries[WeaponIndex].WeaponDataClass.GetDefaultObject())
	{
		GetMesh()->SetAnimInstanceClass(WeaponCDO->AnimInstanceClass);
	}
}

void ASFCharacter::Multicast_SetAnimation_Implementation()
{
	if (WeaponIndex == INDEX_NONE)
		return;

	if (USFWeaponData* WeaponData = Inventory.Entries[WeaponIndex].Instance)
	{
		GetMesh()->SetAnimInstanceClass(WeaponData->AnimInstanceClass);
	}
}

void ASFCharacter::Multicast_SpawnWeapon_Implementation()
{

}

void ASFCharacter::UnequipWeapon()
{
	//Inventory.UnequipWeapon(Index);
	if (WeaponIndex == INDEX_NONE)
		return;
	
	USFWeaponData* WeaponData = Inventory.Entries[WeaponIndex].Instance;
	if (WeaponData->SpawnedWeapon)
	{
		WeaponData->SpawnedWeapon->OnUnequipped();
		WeaponData->SpawnedWeapon->Destroy();
		WeaponData->SpawnedWeapon = nullptr;
	}

	Inventory.RemoveEntry(WeaponData);
	WeaponData = nullptr;
}

void ASFCharacter::UseWeapon()
{
	AGameStateBase* State = GetWorld()->GetGameState();
	float RequestTime = State->GetServerWorldTimeSeconds();

	if (!HasAuthority())
	{
		UseWeaponInternal(RequestTime);
	}
	Server_UseWeapon(RequestTime);
}


void ASFCharacter::Server_UseWeapon_Implementation(float RequestTime)
{
	UseWeaponInternal(RequestTime);
	Multicast_UseWeapon(RequestTime);
}

void ASFCharacter::Multicast_UseWeapon_Implementation(float RequestTime)
{
	if (!IsLocallyControlled())
		UseWeaponInternal(RequestTime);
}

void ASFCharacter::UseWeaponInternal(float AttackTime)
{
	if (WeaponIndex == INDEX_NONE)
		return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}


void ASFCharacter::OnDamage(uint8 Damage, AActor* InInstigator)
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

