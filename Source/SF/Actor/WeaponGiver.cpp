// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WeaponGiver.h"

#include "Character/SFCharacter.h"
#include "Weapon/SFWeaponData.h"

// Sets default values
AWeaponGiver::AWeaponGiver()
{

}

void AWeaponGiver::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponGiver::GiveWeapon(ASFCharacter* Character)
{
	if (Character)
	{
		for (TSubclassOf<USFWeaponData> WeaponData : WeaponClass)
		{
			Character->AddWeapon(WeaponData);
		}
	}
}


