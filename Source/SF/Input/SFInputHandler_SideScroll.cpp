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

	// 2D 평면에서 각도 계산 (횡스크롤 → 보통 X-Z 또는 X-Y 기준)
	FVector CharacterLocation = Character->GetActorLocation() + FVector(0.f, 0.f, 60.f);
	FVector CameraLocation = SideCamera->GetActorLocation();
	FVector Direction = (MouseWorldPosition - CameraLocation).GetSafeNormal(); // 정규화된 방향
	FVector ProjectedPosition;

	if (FMath::Abs(Direction.Y) > KINDA_SMALL_NUMBER)
	{
		// 2. 캐릭터의 Y좌표에 도달할 때까지 연장할 거리(t) 계산
		float t = (CharacterLocation.Y - CameraLocation.Y) / Direction.Y;
		// 3. 최종 투영 위치 계산
		ProjectedPosition = CameraLocation + Direction * t;
	}

	FVector2D ToProjected;
	ToProjected.X = ProjectedPosition.X - CharacterLocation.X;
	ToProjected.Y = ProjectedPosition.Z - CharacterLocation.Z;

	// 2. 캐릭터 Forward 벡터 (X-Z 평면)
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector2D Forward2D = FVector2D(ForwardVector.X, ForwardVector.Z);

	// 3. 정규화
	ToProjected.Normalize();
	Forward2D.Normalize();

	// 4. Dot Product
	float Dot = FVector2D::DotProduct(Forward2D, ToProjected);
	Dot = FMath::Clamp(Dot, -1.0f, 1.0f); // 안전성 확보

	// 5. 라디안 각도 구하기
	float AngleRad = FMath::Acos(Dot);

	// 6. 부호(Sign) 판별
	float Sign = FMath::Sign(ToProjected.Y);

	// 7. 최종 각도 (Degrees)
	float AimAngle = FMath::RadiansToDegrees(AngleRad) * Sign;

	// 8. Clamp (선택사항)
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
		1.0f, // 지속 시간 (초)
		0,
		2.0f // 두께
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

