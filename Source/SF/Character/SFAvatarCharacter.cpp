// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFAvatarCharacter.h"

void ASFAvatarCharacter::SetTarget(ASFPlayerCharacter* InTargetCharacter)
{
	TargetCharacter = InTargetCharacter;
	SetColor();
}

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

FVector ASFAvatarCharacter::GetPlayerThemeOffset() const
{
	if (bFutureTheme)
	{
		return FVector(500.f, 0, 0);
	}
	else
	{
		return FVector(500.f, 0, 0); // Medieval Å×¸¶ Offset
	}
}
