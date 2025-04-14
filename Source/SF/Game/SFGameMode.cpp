// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFGameMode.h"
#include "Character/SFCharacter.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/SFGameInstance.h"

ASFGameMode::ASFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (USFGameInstance* SFGameInstance = Cast<USFGameInstance>(GetGameInstance()))
	{
		FString Error;
		SFGameInstance->CreateLocalPlayer(NewPlayer->PlayerCameraManager, Error);
	}
	//GetGameInstance()->CreateLocalPlayer(NewPlayer->GetNetConnection()->GetConnectionId(), Error, false);
}

