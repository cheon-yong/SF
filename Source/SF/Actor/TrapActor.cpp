// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TrapActor.h"
#include "Character/SFCharacter.h"

// Sets default values
ATrapActor::ATrapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATrapActor::GiveDamage(ASFCharacter* SFCharacter)
{
	if (SFCharacter == nullptr)
	{
		return;
	}

	SFCharacter->OnDamage(Damage, this);
}