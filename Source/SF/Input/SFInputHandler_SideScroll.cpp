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

	// 마우스 위치까지 직선 거리 확장
	FVector MouseWorldPos = WorldLocation + WorldDirection * 10000.f;

	// 2D 평면에서 각도 계산 (횡스크롤 → 보통 X-Z 또는 X-Y 기준)
	FVector CharacterLocation = Character->GetActorLocation();

	FVector2D ToMouse;
	ToMouse.X = MouseWorldPos.X - CharacterLocation.X;
	ToMouse.Y = MouseWorldPos.Z - CharacterLocation.Z; // 횡스크롤에서는 Z축이 화면 위쪽인 경우

	FVector ToMouseVector = FVector(ToMouse.X, CharacterLocation.Y, ToMouse.Y);

	FVector ForwardVector = Character->GetActorForwardVector();
	FVector2D Forward = FVector2D(ForwardVector.X, ForwardVector.Z);

	ToMouse.Normalize();
	Forward.Normalize();

	// 두 벡터 사이의 각도 (라디안)
	float AngleRad = FMath::Acos(FVector2D::DotProduct(Forward, ToMouse));

	// 부호 판단 (위쪽 조준인지 아래 조준인지)
	float Sign = FMath::Sign(ToMouse.Y);

	// 최종 Pitch (도 단위로 변환, 부호 포함)
	float PitchDeg = FMath::RadiansToDegrees(AngleRad) * Sign;

	// Clamp (AimOffset에서 -90~90으로 제한)
	PitchDeg = FMath::Clamp(PitchDeg, -90.f, 90.f);

	Pitch = PitchDeg;
	if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(Character))
	{
		SFCharacter->Pitch_SideScroll = Pitch;
		SFCharacter->ToMouseVector = ToMouseVector;
	}

	// 예: 로그 출력
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
		

		// 횡스크롤에서는 일반적으로 X축 또는 Y축 한쪽으로만 이동
		// 예: X축 방향이 오른쪽이라면
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
	FVector LaunchDirection = -Forward; // 벽의 반대 방향
	LaunchDirection.Z = 1.0f; // 위로도 튕기게

	FVector LaunchVelocity = LaunchDirection * 600.0f; // 속도 조절

	if (WallJumpMontage)
		GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Play(WallJumpMontage);

	GetCharacter()->LaunchCharacter(LaunchVelocity, true, true);
	//GetCharacter()->GetMesh()->PlayAni
}

bool USFInputHandler_SideScroll::WallInFront()
{
	// Mesh의 발을 기준으로 점프 가능을 판단하기 위해서 Mesh를 기준으로 Trace
	FVector Start = GetCharacter()->GetMesh()->GetComponentLocation();
	FVector Forward = GetCharacter()->GetMesh()->GetRightVector(); // 현재 Mesh가 회전되어있기 때문에 RightVector 사용
	FVector End = Start + Forward * 60.0f; // 60cm 앞까지 검사

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, GetCharacter());

	TraceParams.bReturnPhysicalMaterial = false;

	// 라인트레이스 수행
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