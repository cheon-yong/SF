// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFRangeWeapon.h"
#include "Weapon/SFProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/SFPlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ASFRangeWeapon::Attack()
{
	if (ProjectileClass)
	{
		FVector Location = MeshComp->GetSocketLocation(SpawnSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = GetInstigator();
		
		FRotator ProjectileRotation;
		FVector Direction;

		// TODO : 변경할 것
		if (ASFPlayerCharacter* SFCharacter = Cast<ASFPlayerCharacter>(Owner))
		{
			float Angle = Owner->GetActorForwardVector().X < 0 ? SFCharacter->Pitch_SideScroll : SFCharacter->Pitch_SideScroll * -1;
			Direction = Owner->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 1, 0));
		}

		ProjectileRotation = Direction.Rotation();

		ASFProjectile* Projectile = GetWorld()->SpawnActor<ASFProjectile>(ProjectileClass,
			Location,
			ProjectileRotation,
			SpawnParams
		);

		Projectile->Damage = 1;

		OnFire.Broadcast();
	}
}