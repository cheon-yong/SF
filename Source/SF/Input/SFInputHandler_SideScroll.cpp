// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SFInputHandler_SideScroll.h"

#include "Character/SFPlayerCharacter.h"
#include "Player/SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Character/SFCharacter.h>
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"
#include "Camera/SFCameraActor_SideScroll.h"
#include "Player/LocalPlayerBlueprintLibrary.h"

USFInputHandler_SideScroll::USFInputHandler_SideScroll()
{
	Pitch = 0;
}

void USFInputHandler_SideScroll::Bind(ASFPlayerController* PlayerController, UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::Bind(PlayerController, EnhancedInputComponent);

	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	// Shoot 
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ThisClass::Shoot);



	OriginRate = GetCharacter()->GetCharacterMovement()->RotationRate;

	GetCharacter()->GetCharacterMovement()->RotationRate = TargetRotationRate;
	GetCharacter()->GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::XZPlane);
}

void USFInputHandler_SideScroll::Unbind()
{
	GetCharacter()->GetCharacterMovement()->RotationRate = OriginRate;

	Super::Unbind();
}

void USFInputHandler_SideScroll::Tick(float DeltaSeconds)
{
	if (SideCamera == nullptr)
	{
		for (TActorIterator<ASFCameraActor_SideScroll> It(GetWorld()); It; ++It)
		{
			SideCamera = *It;
		}
	}

	SetAimOffset();
}

void USFInputHandler_SideScroll::SetAimOffset()
{
	// Get Mouse Location in world
	FVector2D MousePosition;
	FVector WorldLocation, WorldDirection;

	if (SFPlayerController == nullptr || SFPlayerController->bMainController == false || SideCamera == nullptr)
	{
		return;
	}

	FVector MouseWorldPosition;
	FHitResult HitResult;
	FVector2D ClientMousePos;
	if (SFPlayerController->HasAuthority())
	{
		if (SFPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			MouseWorldPosition = HitResult.Location;
		}
	}
	else
	{
		if (SFPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			MouseWorldPosition = HitResult.Location;
		}

		const TArray<class ULocalPlayer*> LocalPlayers = GetWorld()->GetGameInstance()->GetLocalPlayers();
		for (auto LC : LocalPlayers)
		{
			if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(LC->PlayerController))
			{
				if (SFPC->bMainController == false)
				{
					if (LC->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
					{
						MouseWorldPosition = HitResult.Location;
					}
					//SFPlayerController->GetMousePosition(ClientMousePos.X, ClientMousePos.Y);
				}
			}
		}
	}

	// Get Direct Vector
	ACharacter* Character = SFPlayerController->GetCharacter();
	if (Character == nullptr)
	{
		return;
	}

	// 2D ��鿡�� ���� ��� (Ⱦ��ũ�� �� ���� X-Z �Ǵ� X-Y ����)
	FVector CharacterLocation = Character->GetActorLocation() + FVector(0.f, 0.f, 60.f);
	FVector CameraLocation = SideCamera->GetActorLocation();
	FVector Direction = (MouseWorldPosition - CameraLocation).GetSafeNormal(); // ����ȭ�� ����
	FVector ProjectedPosition;

	if (FMath::Abs(Direction.Y) > KINDA_SMALL_NUMBER)
	{
		// 2. ĳ������ Y��ǥ�� ������ ������ ������ �Ÿ�(t) ���
		float t = (CharacterLocation.Y - CameraLocation.Y) / Direction.Y;
		// 3. ���� ���� ��ġ ���
		ProjectedPosition = CameraLocation + Direction * t;
	}

	FVector2D ToProjected;
	ToProjected.X = ProjectedPosition.X - CharacterLocation.X;
	ToProjected.Y = ProjectedPosition.Z - CharacterLocation.Z;

	// 2. ĳ���� Forward ���� (X-Z ���)
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector2D Forward2D = FVector2D(ForwardVector.X, ForwardVector.Z);

	// 3. ����ȭ
	ToProjected.Normalize();
	Forward2D.Normalize();

	// 4. Dot Product
	float Dot = FVector2D::DotProduct(Forward2D, ToProjected);
	Dot = FMath::Clamp(Dot, -1.0f, 1.0f); // ������ Ȯ��

	// 5. ���� ���� ���ϱ�
	float AngleRad = FMath::Acos(Dot);

	// 6. ��ȣ(Sign) �Ǻ�
	float Sign = FMath::Sign(ToProjected.Y);

	// 7. ���� ���� (Degrees)
	float AimAngle = FMath::RadiansToDegrees(AngleRad) * Sign;

	// 8. Clamp (���û���)
	AimAngle = FMath::Clamp(AimAngle, -90.f, 90.f);

	PrevPitch = Pitch;
	Pitch = AimAngle;

	if (FMath::Abs(PrevPitch - Pitch) >= AimAngleUpdateThreshold)
	{
		if (SFPlayerController->HasAuthority())
		{
			if (ASFPlayerCharacter* SFCharacter = Cast<ASFPlayerCharacter>(Character))
			{
				SFCharacter->Pitch_SideScroll = Pitch;
			}
		}
		else
		{
			if (ASFPlayerCharacter* SFCharacter = Cast<ASFPlayerCharacter>(Character))
			{
				SFCharacter->Server_UpdateAim(Pitch);
			}
		}
	}
}


void USFInputHandler_SideScroll::DrawDebug2DVector(const FVector2D& Vector2D, const FVector& Origin, float Scale, FColor Color)
{
	FVector Start = Origin;
	FVector End = Origin + FVector(Vector2D.X, 0.0f, Vector2D.Y) * Scale;

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		Color,
		false,
		1.0f, // ���� �ð� (��)
		0,
		2.0f // �β�
	);
}

void USFInputHandler_SideScroll::Shoot()
{
	if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(GetCharacter()))
	{
		SFCharacter->UseWeapon();
	}
}

void USFInputHandler_SideScroll::Move(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// add movement 

		// Ⱦ��ũ�ѿ����� �Ϲ������� X�� �Ǵ� Y�� �������θ� �̵�
		// ��: X�� ������ �������̶��
		ControlledPawn->AddMovementInput(FVector::BackwardVector, Direction);
	}
}

void USFInputHandler_SideScroll::Look(const FInputActionValue& Value)
{
}

void USFInputHandler_SideScroll::Jump()
{
	bool bIsFalling = GetCharacter()->GetMovementComponent()->IsFalling();

	
	if (bIsFalling && WallInFront())
	{
		WallJump();
		return;
	}

	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void USFInputHandler_SideScroll::WallJump()
{
	FVector Forward = GetCharacter()->GetActorForwardVector();
	FVector LaunchDirection = -Forward; // ���� �ݴ� ����
	LaunchDirection.Z = 1.0f; // ���ε� ƨ���

	FVector LaunchVelocity = LaunchDirection * 600.0f; // �ӵ� ����

	if (WallJumpMontage)
		GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Play(WallJumpMontage);

	GetCharacter()->LaunchCharacter(LaunchVelocity, true, true);
	//GetCharacter()->GetMesh()->PlayAni
}

bool USFInputHandler_SideScroll::WallInFront()
{
	// Mesh�� ���� �������� ���� ������ �Ǵ��ϱ� ���ؼ� Mesh�� �������� Trace
	FVector Start = GetCharacter()->GetMesh()->GetComponentLocation();
	FVector Forward = GetCharacter()->GetMesh()->GetRightVector(); // ���� Mesh�� ȸ���Ǿ��ֱ� ������ RightVector ���
	FVector End = Start + Forward * 60.0f; // 60cm �ձ��� �˻�

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, GetCharacter());

	TraceParams.bReturnPhysicalMaterial = false;

	// ����Ʈ���̽� ����
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		Start,
		End,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		TraceParams
	);

	return bHit;
	
}

void USFInputHandler_SideScroll::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

