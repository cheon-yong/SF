// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFAvatarCharacter.h"
#include "Animation/SFAnimInstance.h"
#include "Animation/SFAnimInstance_Copy.h"
#include "Net/UnrealNetwork.h"

void ASFAvatarCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASFAvatarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter != nullptr)
	{
		CopyState();
	}
}

void ASFAvatarCharacter::SetTarget(ASFPlayerCharacter* InTargetCharacter)
{
	TargetCharacter = InTargetCharacter;

	SetTargetAnimInstance();
	SetColor();
}

void ASFAvatarCharacter::SetTargetAnimInstance()
{
	if (TargetCharacter == nullptr)
		return;

	if (USFAnimInstance* TargetAnimClass = Cast<USFAnimInstance>(TargetCharacter->GetMesh()->GetAnimInstance()))
	{
		TargetAnim = TargetAnimClass;
		if (USFAnimInstance_Copy* CurrentAnim = Cast<USFAnimInstance_Copy>(GetMesh()->GetAnimInstance()))
		{
			AvartarAnim = CurrentAnim;
			AvartarAnim->TargetAnim = TargetAnimClass;
		}
	}
}

void ASFAvatarCharacter::SetColor()
{
	if (TargetCharacter)
	{
		auto Mat0 = TargetCharacter->GetMesh()->GetMaterial(0);
		auto Mat1 = TargetCharacter->GetMesh()->GetMaterial(1);

		GetMesh()->SetMaterial(0, Mat0);
		GetMesh()->SetMaterial(1, Mat1);
	}
}

void ASFAvatarCharacter::CopyState()
{
	SetActorLocation(TargetCharacter->GetActorLocation() + GetPlayerThemeOffset());
	SetActorRotation(TargetCharacter->GetActorRotation());
}

void ASFAvatarCharacter::OnRep_TargetCharacter()
{
	SetTargetAnimInstance();
	SetColor();
}

FVector ASFAvatarCharacter::GetPlayerThemeOffset() const
{
	if (bFutureTheme)
	{
		return Offset;
	}
	else
	{
		return -Offset; // Medieval Å×¸¶ Offset
	}
}

void ASFAvatarCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TargetCharacter);
	DOREPLIFETIME(ThisClass, bFutureTheme);
}
