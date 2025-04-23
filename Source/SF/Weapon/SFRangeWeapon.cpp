// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFRangeWeapon.h"
#include "Weapon/SFProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/SFCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ASFRangeWeapon::Attack()
{
	if (ProjectileClass)
	{
		FTransform SpawnTransform = MeshComp->GetSocketByName(SpawnSocketName)->GetSocketTransform(MeshComp);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = GetInstigator();
		
		FRotator ProjectileRotation;
		FVector Direction;
		if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(Owner))
		{
			Direction = Owner->GetActorForwardVector().RotateAngleAxis(SFCharacter->Pitch_SideScroll, FVector(0, 1, 0));
		}

		ProjectileRotation = Direction.Rotation();

		GetWorld()->SpawnActor<ASFProjectile>(ProjectileClass,
			SpawnTransform.GetLocation(),
			ProjectileRotation,
			SpawnParams
		);

		OnFire.Broadcast();
	}
}