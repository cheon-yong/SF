// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SFCameraActor_SideScroll.h"
#include "Camera/CameraComponent.h"

#include "Character/SFPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"

#include "EngineUtils.h"

ASFCameraActor_SideScroll::ASFCameraActor_SideScroll()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	GetCameraComponent()->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	GetCameraComponent()->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bActive = false;


}

void ASFCameraActor_SideScroll::SetActive(bool Active)
{
	bActive = Active;
}

void ASFCameraActor_SideScroll::GetPlayers()
{
	for (TActorIterator<ASFPlayerCharacter> It(GetWorld()); It; ++It)
	{
		ASFPlayerCharacter* FoundActor = *It;
		if (FoundActor)
		{
			Players.AddUnique(FoundActor);
		}
	}
}

void ASFCameraActor_SideScroll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActive == false)
		return;

	GetTargetValues();
	SetInterpolationValue(DeltaTime);
}

void ASFCameraActor_SideScroll::GetTargetValues()
{
	TargetLocation = CalculateAverageLocation();
	TargetArmLength = CalculateArmLength();
}

FVector ASFCameraActor_SideScroll::CalculateAverageLocation()
{
	if (Players.Num() == 0)
	{
		return GetActorLocation();
	}
		
	FVector Sum = FVector::Zero();
	for (ACharacter* Character : Players)
	{
		Sum += Character->GetActorLocation();
	}

	FVector NewTargetPos = Sum / Players.Num();
	NewTargetPos.Y = GetActorLocation().Y;
	return NewTargetPos;
}

float ASFCameraActor_SideScroll::CalculateArmLength()
{
	float Distance = GetPlayersDistance();

	float DesiredZoom = FMath::Clamp(Distance, MinArmLength, MaxArmLength);

	return DesiredZoom;
}

float ASFCameraActor_SideScroll::GetPlayersDistance()
{
	if (Players.Num() < 2)
		return 0;

	return FVector::Distance(Players[0]->GetActorLocation(), Players[1]->GetActorLocation());
}

void ASFCameraActor_SideScroll::SetInterpolationValue(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, CameraMoveSpeed);
	SetActorLocation(NewLocation);

	float NewArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaTime, ZoomSpeed);
	CameraBoom->TargetArmLength = NewArmLength;
}
