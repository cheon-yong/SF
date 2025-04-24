// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SFInputHandler_SideScroll.h"

#include "Character/SFCharacter.h"
#include "Player/SFPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Character/SFCharacter.h>
#include "CollisionQueryParams.h"

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
}

void USFInputHandler_SideScroll::Unbind()
{
	GetCharacter()->GetCharacterMovement()->RotationRate = OriginRate;

	Super::Unbind();
}

void USFInputHandler_SideScroll::Tick(float DeltaSeconds)
{
	SetAimOffset();
}

void USFInputHandler_SideScroll::SetAimOffset()
{
	// Get Mouse Location in world
	FVector2D MousePosition;
	FVector WorldLocation, WorldDirection;
	SFPlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	SFPlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

	// Get Direct Vector
	ACharacter* Character = SFPlayerController->GetCharacter();

	// ���콺 ��ġ���� ���� �Ÿ� Ȯ��
	FVector MouseWorldPos = WorldLocation + WorldDirection * 10000.f;

	// 2D ��鿡�� ���� ��� (Ⱦ��ũ�� �� ���� X-Z �Ǵ� X-Y ����)
	FVector CharacterLocation = Character->GetActorLocation();

	FVector2D ToMouse;
	ToMouse.X = MouseWorldPos.X - CharacterLocation.X;
	ToMouse.Y = MouseWorldPos.Z - CharacterLocation.Z; // Ⱦ��ũ�ѿ����� Z���� ȭ�� ������ ���

	FVector ToMouseVector = FVector(ToMouse.X, CharacterLocation.Y, ToMouse.Y);

	FVector ForwardVector = Character->GetActorForwardVector();
	FVector2D Forward = FVector2D(ForwardVector.X, ForwardVector.Z);

	ToMouse.Normalize();
	Forward.Normalize();

	// �� ���� ������ ���� (����)
	float AngleRad = FMath::Acos(FVector2D::DotProduct(Forward, ToMouse));

	// ��ȣ �Ǵ� (���� �������� �Ʒ� ��������)
	float Sign = FMath::Sign(ToMouse.Y);

	// ���� Pitch (�� ������ ��ȯ, ��ȣ ����)
	float PitchDeg = FMath::RadiansToDegrees(AngleRad) * Sign;

	// Clamp (AimOffset���� -90~90���� ����)
	PitchDeg = FMath::Clamp(PitchDeg, -90.f, 90.f);

	Pitch = PitchDeg;
	if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(Character))
	{
		SFCharacter->Pitch_SideScroll = Pitch;
		SFCharacter->ToMouseVector = ToMouseVector;
	}

	// ��: �α� ���
	//UE_LOG(LogTemp, Log, TEXT("Aim Angle: %.2f degrees"), AngleDeg);
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