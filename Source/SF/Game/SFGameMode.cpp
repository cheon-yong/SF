// Copyright Epic Games, Inc. All Rights Reserved.

#include "SFGameMode.h"
#include "Character/SFCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASFGameMode::ASFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
