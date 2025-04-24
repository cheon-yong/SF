// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFNonPlayerCharacter.h"
#include "Weapon/SFProjectile.h"

// Sets default values
ASFNonPlayerCharacter::ASFNonPlayerCharacter()
{
}

// Called when the game starts or when spawned
void ASFNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASFNonPlayerCharacter::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		OnAttackFinished.ExecuteIfBound();

	}
}

float ASFNonPlayerCharacter::GetAIPatrolRadius()
{
	return PatrolRadius;
}

float ASFNonPlayerCharacter::GetAITurnSpeed()
{
	return TurnSpeed;
}

float ASFNonPlayerCharacter::GetAIAttackRange()
{
	return AttackRange;
}

float ASFNonPlayerCharacter::GetAIDetectRadius()
{
	return DetectRadius;
}

void ASFNonPlayerCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASFNonPlayerCharacter::AttackByAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage);
	AnimInstance->OnMontageEnded.Clear();
	AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackEnd);
}

void ASFNonPlayerCharacter::CreateProjectile()
{
	if (ProjectileClass)
	{
		FVector Location = GetMesh()->GetSocketLocation(SpawnSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = GetInstigator();

		if (nullptr == Target)
		{
			return;
		}

		FRotator ProjectileRotation;
		FVector Direction = Target->GetActorLocation() - GetActorLocation();

		ProjectileRotation = Direction.Rotation();

		GetWorld()->SpawnActor<ASFProjectile>(ProjectileClass,
			Location,
			ProjectileRotation,
			SpawnParams
		);
	}
}
