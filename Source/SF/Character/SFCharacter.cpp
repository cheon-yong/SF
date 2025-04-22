// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Game/SFGameMode.h>
#include "Weapon/SFWeapon.h"
#include "Weapon/SFWeaponData.h"

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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	Pitch_SideScroll = 0;
}

void ASFCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SetColor();
}


void ASFCharacter::AddWeapon(TSubclassOf<USFWeaponData> WeaponDataClass)
{
	USFWeaponData* NewWeaponData = NewObject<USFWeaponData>(this, WeaponDataClass);

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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	ASFWeapon* NewWeapon = GetWorld()->SpawnActor<ASFWeapon>(WeaponData->WeaponClass, FVector::ZeroVector, FRotator(0.f, -90.f, 0.f), SpawnParams);
	if (NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponData->SocketName);
		CurrentWeapon = WeaponData;
		NewWeapon->OnEquipped();
		WeaponData->SpawnedWeapon = NewWeapon;
		
	}

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


void ASFCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}


void ASFCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

}

void ASFCharacter::SetColor()
{

	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		if (IsLocallyControlled())
		{
			GetMesh()->SetMaterial(0, CharacterColorMaterials[(int)ECharacterColor::Blue].Material0);
			GetMesh()->SetMaterial(1, CharacterColorMaterials[(int)ECharacterColor::Blue].Material1);
		}
		else
		{
			GetMesh()->SetMaterial(0, CharacterColorMaterials[(int)ECharacterColor::Red].Material0);
			GetMesh()->SetMaterial(1, CharacterColorMaterials[(int)ECharacterColor::Red].Material1);
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
			GetMesh()->SetMaterial(0, CharacterColorMaterials[(int)ECharacterColor::Red].Material0);
			GetMesh()->SetMaterial(1, CharacterColorMaterials[(int)ECharacterColor::Red].Material1);
		}
		else
		{
			GetMesh()->SetMaterial(0, CharacterColorMaterials[(int)ECharacterColor::Blue].Material0);
			GetMesh()->SetMaterial(1, CharacterColorMaterials[(int)ECharacterColor::Blue].Material1);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
}