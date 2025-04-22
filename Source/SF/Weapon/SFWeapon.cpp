// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASFWeapon::ASFWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = MeshComp;
}

void ASFWeapon::Shoot()
{
}

void ASFWeapon::OnEquipped()
{
}

void ASFWeapon::OnUnequipped()
{
}

// Called when the game starts or when spawned
void ASFWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

