// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SFNonPlayerCharacter.h"
#include "Weapon/SFProjectile.h"
#include "Widget/SFWidgetComponent.h"
#include "Widget/SFUserWidget.h"
#include "Components/SphereComponent.h"

// Sets default values
ASFNonPlayerCharacter::ASFNonPlayerCharacter()
	: Super()
{
	HpBar = CreateDefaultSubobject<USFWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	if (HpBarClass)
	{
		HpBar->SetWidgetClass(HpBarClass);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(500.0f, 500.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

		ASFProjectile* Projectile = GetWorld()->SpawnActor<ASFProjectile>(ProjectileClass,
			Location,
			ProjectileRotation,
			SpawnParams
		);

		Projectile->GetCollisionComp()->IgnoreActorWhenMoving(this, true);
	}
}

void ASFNonPlayerCharacter::OnDamage(uint8 Damage, AActor* InInstigator)
{
	Super::OnDamage(Damage, InInstigator);
}
