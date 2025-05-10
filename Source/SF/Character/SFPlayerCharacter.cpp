// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFPlayerCharacter.h"

#include "Actor/EffectActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include <Net/UnrealNetwork.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Actor/InteractActor.h"


ASFPlayerCharacter::ASFPlayerCharacter() 
	: Super()
{
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

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(RootComponent);

	Pitch_SideScroll = 0;
	ToMouseVector = FVector::Zero();
}

void ASFPlayerCharacter::ChangeCameraComponent()
{
	
}

void ASFPlayerCharacter::Respawn()
{
	OnSpawned.Broadcast();
}

TArray<AActor*> ASFPlayerCharacter::GetInteractActors()
{
	TArray<AActor*> OverlappingActors;
	InteractBox->GetOverlappingActors(OverlappingActors);

	return OverlappingActors;
}

void ASFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetColor();

	OnSpawned.Broadcast();

	InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractBoxBeginOverlap);
	InteractBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractBoxEndOverlap);
}

void ASFPlayerCharacter::SetColor()
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

void ASFPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Pitch_SideScroll);
}


void ASFPlayerCharacter::Server_UpdateAim_Implementation(float NewPitch)
{
	Pitch_SideScroll = NewPitch;
}


void ASFPlayerCharacter::OnRep_Pitch()
{
	
}

void ASFPlayerCharacter::OnSpawn()
{
	Super::OnSpawn();

	if (SpawnEffectClass)
	{
		if (DeathEffect)
		{
			DeathEffect->Destroy();
			DeathEffect = nullptr;
		}

		if (SpawnEffect != nullptr)
		{
			SpawnEffect->Destroy();
			SpawnEffect = nullptr;
		}

		FActorSpawnParameters SpawnParams;
		SpawnEffect = GetWorld()->SpawnActor<AActor>(SpawnEffectClass,
			GetMesh()->GetComponentLocation(),
			GetActorRotation(),
			SpawnParams
		);

		if (AEffectActor* SEA = Cast<AEffectActor>(SpawnEffect))
		{
			SEA->Burst(this);
		}
	}
}

void ASFPlayerCharacter::OnDeath()
{
	if (HasAuthority() == false)
		return;

	if (DeathEffectClass)
	{
		FActorSpawnParameters SpawnParams;
		if (DeathEffect != nullptr)
		{
			DeathEffect->Destroy();
			DeathEffect = nullptr;
		}
		DeathEffect = GetWorld()->SpawnActor<AActor>(DeathEffectClass,
			GetMesh()->GetComponentLocation(),
			GetActorRotation(),
			SpawnParams
		);

		//DeathEffect->SetLifeSpan(0.8f);

		if (AEffectActor* DEA = Cast<AEffectActor>(DeathEffect))
		{
			DEA->Burst(this);
		}
	}

	//Super::OnDeath();
}

void ASFPlayerCharacter::OnInteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInteractActor* InteractActor = Cast<AInteractActor>(OtherActor))
	{
		InteractActor->ShowInteractWidget(true);
	}
}

void ASFPlayerCharacter::OnInteractBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractActor* InteractActor = Cast<AInteractActor>(OtherActor))
	{
		InteractActor->ShowInteractWidget(false);
	}
}
