// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFWeaponData.h"
#include "Character/SFCharacter.h"
#include "Weapon/SFWeapon.h"

void USFWeaponData::SetOwner(AActor* InOwner)
{
	Owner = InOwner;
}

void USFWeaponData::SetInstigator(APawn* InInstigator)
{
	Instigator = InInstigator;
}

ASFWeapon* USFWeaponData::SpawnWeapon(ASFCharacter* InOwner)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InOwner;
	SpawnParams.Instigator = Instigator;

	ASFWeapon* NewWeapon = GetWorld()->SpawnActor<ASFWeapon>(WeaponClass, FVector::ZeroVector, FRotator(0.f, -90.f, 0.f), SpawnParams);
	if (NewWeapon)
	{
		NewWeapon->AttachToComponent(InOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		NewWeapon->OnEquipped();
		SpawnedWeapon = NewWeapon;
	}

	return NewWeapon;
}
