// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFGameMode.h"
#include "Character/SFCharacter.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/SFPlayerController.h"
#include "Game/SFGameInstance.h"
#include "Camera/ViewportBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"

ASFGameMode::ASFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASFGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->IsLocalController())
		return;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (GameInstance->GetNumLocalPlayers() < 2)
		{
			FString Error;
			//SFGameInstance->CreateLocalPlayer(NewPlayer, Error);
			//SFGameInstance->CreateLocalPlayer(NewPlayer->PlayerCameraManager, Error);
			int32 NewId = GameInstance->GetLocalPlayerByIndex(0)->GetControllerId() + 1;

			ULocalPlayer* EmptyLocalPlayer = GameInstance->CreateLocalPlayer(NewId, Error, true);
			if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(EmptyLocalPlayer->GetPlayerController(GetWorld())))
			{
				SFPC->bMainController = false;
				ACharacter* EmptyChar = SFPC->GetCharacter();
				GetWorld()->DestroyActor(EmptyChar);
				SFPC->SetViewTarget(NewPlayer->GetPawn());

				UViewportBlueprintFunctionLibrary::SetSplitScreenType(ESFSplitScreenType::SeparateScreen);
			}
		}
	}
}

AActor* ASFGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	for (AActor* Start : PlayerStarts)
	{
		if (Start->ActorHasTag("ServerStart") && Player->IsLocalController())
		{
			return Start;
		}
		else if (Start->ActorHasTag("ClientStart") && !Player->IsLocalController())
		{
			return Start;
		}
	}

	// fallback
	return Super::ChoosePlayerStart_Implementation(Player);
}

