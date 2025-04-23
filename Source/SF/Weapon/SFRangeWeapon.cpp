// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFRangeWeapon.h"
#include "Weapon/SFProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/SFCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

UE_DISABLE_OPTIMIZATION
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
		if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(Owner))
		{
			float Angle = Owner->GetActorForwardVector().X < 0 ? SFCharacter->Pitch_SideScroll : SFCharacter->Pitch_SideScroll * -1;
			Direction = Owner->GetActorForwardVector().RotateAngleAxis(Angle, FVector(0, 1, 0));
		}

		ProjectileRotation = Direction.Rotation();

		GetWorld()->SpawnActor<ASFProjectile>(ProjectileClass,
			Location,
			ProjectileRotation,
			SpawnParams
		);

		OnFire.Broadcast();
	}
}
UE_ENABLE_OPTIMIZATION