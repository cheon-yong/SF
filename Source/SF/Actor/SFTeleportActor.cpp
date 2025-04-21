// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SFTeleportActor.h"
#include "GameFramework/Character.h"

// Sets default values
ASFTeleportActor::ASFTeleportActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASFTeleportActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASFTeleportActor::Server_TeleportActors_Implementation(const TArray<ACharacter*>& Characters)
{
	for (int i = 0; i < Characters.Num(); i++)
	{
		const FVector& NewVector = TeleportPoints[i]->GetActorLocation();

		Characters[i]->SetActorLocation(NewVector, false, nullptr, ETeleportType::TeleportPhysics);
	}
}
