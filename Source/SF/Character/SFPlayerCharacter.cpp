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
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Actor/InteractActor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


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

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

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

void ASFPlayerCharacter::Interact()
{

	if (!HasAuthority())
	{
		Server_Interact();
		return;
	}

	Interact_Internal();
}

void ASFPlayerCharacter::PlayAnimMontageAndBlockMove(UAnimMontage* MontageToPlay, float PlayRate, FVector StartingPosition, FString StartingSection)
{
	if (HasAuthority())
	{
		Multi_PlayAnimMontageAndBlockMove(MontageToPlay, PlayRate, StartingPosition, StartingSection);
	}
	else
	{
		Server_PlayAnimMontageAndBlockMove(MontageToPlay, PlayRate, StartingPosition, StartingSection);
	}
}

void ASFPlayerCharacter::Multi_PlayAnimMontageAndBlockMove_Implementation(UAnimMontage* MontageToPlay, float PlayRate, FVector StartingPosition, const FString& StartingSection)
{
	PlayAnimMontageAndBlockMove_Internal(MontageToPlay, PlayRate, StartingPosition, StartingSection);
}

void ASFPlayerCharacter::Server_PlayAnimMontageAndBlockMove_Implementation(UAnimMontage* MontageToPlay, float PlayRate, FVector StartingPosition, const FString& StartingSection)
{
	Multi_PlayAnimMontageAndBlockMove(MontageToPlay, PlayRate, StartingPosition, StartingSection);
}

void ASFPlayerCharacter::PlayAnimMontageAndBlockMove_Internal(UAnimMontage* MontageToPlay, float PlayRate, FVector StartingPosition, FString StartingSection)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay, PlayRate);
	FOnMontageEnded EndDelegate;

	EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		});

	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
}

void ASFPlayerCharacter::Interact_Internal()
{
	TArray<AActor*> Actors = GetInteractActors();

	AInteractActor* PrimaryActor = nullptr;
	float MinDist = FLT_MAX;
	for (AActor* Actor : Actors)
	{
		if (AInteractActor* IA = Cast<AInteractActor>(Actor))
		{
			float Dist = IA->GetDistanceTo(this);
			if (Dist < MinDist)
			{
				PrimaryActor = IA;
				MinDist = Dist;
			}
		}
	}

	if (PrimaryActor == nullptr)
	{
		return;
	}

	PrimaryActor->BeginInteract(this);
}

void ASFPlayerCharacter::Dash_Internal(UAnimMontage* DashMontage, TSubclassOf<AEffectActor> DashEffectClass)
{
	FVector Forward = GetActorForwardVector();
	FVector LaunchDirection = Forward;
	LaunchDirection.Z = 0.2f;

	FVector LaunchVelocity = LaunchDirection * 1500.0f;

	PlayMontage(DashMontage);

	if (DashEffectClass)
	{
		FActorSpawnParameters SpawnParams;
		AEffectActor* DashEffect = GetWorld()->SpawnActor<AEffectActor>(DashEffectClass,
			GetMesh()->GetComponentLocation(),
			GetActorRotation(),
			SpawnParams
		);

		DashEffect->Burst(this);
	}

	LaunchCharacter(LaunchVelocity, true, true);
}

void ASFPlayerCharacter::Server_Dash_Implementation(UAnimMontage* DashMontage, TSubclassOf<AEffectActor> DashEffectClass)
{
	Dash_Internal(DashMontage, DashEffectClass);
}

void ASFPlayerCharacter::WallJump(UAnimMontage* WallJumpMontage)
{
	if (HasAuthority())
	{
		WallJump_Internal(WallJumpMontage);
		return;
	}

	Server_WallJump(WallJumpMontage);
}

void ASFPlayerCharacter::ResetOption()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->RotationRate = OriginRate;
		Multi_ResetOption();
		return;
	}

	Server_ResetOption();
}

void ASFPlayerCharacter::SetSideOption(FRotator TargetRotationRate)
{
	if (HasAuthority())
	{
		OriginRate = GetCharacterMovement()->RotationRate;
		GetCharacterMovement()->RotationRate = TargetRotationRate;

		Multi_SetSideOption(TargetRotationRate);
		return;
	}

	Server_SetSideOption(TargetRotationRate);
}

void ASFPlayerCharacter::PlayMontage(UAnimMontage* Montage)
{
	if (HasAuthority())
	{
		Multi_PlayMontage(Montage);
	}
	else
	{
		Server_PlayMontage(Montage);
	}
}

void ASFPlayerCharacter::WallJump_Internal(UAnimMontage* WallJumpMontage)
{
	FVector Forward = GetActorForwardVector();
	FVector LaunchDirection = -Forward; // 벽의 반대 방향
	LaunchDirection.Z = 1.5f; // 위로도 튕기게

	FVector LaunchVelocity = LaunchDirection * 600.0f; // 속도 조절

	PlayMontage(WallJumpMontage);

	LaunchCharacter(LaunchVelocity, true, true);
}

void ASFPlayerCharacter::Multi_ResetOption_Implementation()
{
	GetMesh()->SetConstraintMode(EDOFMode::Type::None);
	GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::None);
}

void ASFPlayerCharacter::Server_ResetOption_Implementation()
{
	Multi_ResetOption();
}

void ASFPlayerCharacter::Multi_SetSideOption_Implementation(FRotator TargetRotationRate)
{
	GetMesh()->SetConstraintMode(EDOFMode::Type::XZPlane);
	GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::XZPlane);
}

void ASFPlayerCharacter::Multi_PlayMontage_Implementation(UAnimMontage* Montage)
{
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	OnPlayMontage.Broadcast(Montage);
}

void ASFPlayerCharacter::Server_PlayMontage_Implementation(UAnimMontage* Montage)
{
	Multi_PlayMontage(Montage);
}

void ASFPlayerCharacter::Server_SetSideOption_Implementation(FRotator TargetRotationRate)
{
	Multi_SetSideOption(TargetRotationRate);
}

void ASFPlayerCharacter::Server_WallJump_Implementation(UAnimMontage* WallJumpMontage)
{
	WallJump_Internal(WallJumpMontage);
}

void ASFPlayerCharacter::Server_Interact_Implementation()
{
	Interact_Internal();
}

TArray<AActor*> ASFPlayerCharacter::GetInteractActors()
{
	TArray<AActor*> OverlappingActors;
	InteractBox->GetOverlappingActors(OverlappingActors);

	return OverlappingActors;
}

void ASFPlayerCharacter::Dash(UAnimMontage* DashMontage, TSubclassOf<AEffectActor> DashEffectClass)
{
	if (HasAuthority())
	{
		Dash_Internal(DashMontage, DashEffectClass);
		return;
	}

	Server_Dash(DashMontage, DashEffectClass);
}

void ASFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetOwner(this);

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
		if (IsLocallyControlled())
		{
			InteractActor->ShowInteractWidget(true);
		}
	}
}

void ASFPlayerCharacter::OnInteractBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractActor* InteractActor = Cast<AInteractActor>(OtherActor))
	{
		if (IsLocallyControlled())
		{
			InteractActor->ShowInteractWidget(false);
		}
	}
}
