// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LocalPlayerBlueprintLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/GameInstance.h"
#include "Player/SFPlayerController.h"

APlayerController* ULocalPlayerBlueprintLibrary::GetServerControllerInClient(const UObject* WorldContext)
{

	if (WorldContext == nullptr)
		return nullptr;

	UWorld* World = WorldContext->GetWorld();

	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (World->GetGameState()->HasAuthority())
		{
			return nullptr;
		}

		auto LocalPlayers = GameInstance->GetLocalPlayers();
		for (const ULocalPlayer* LC : LocalPlayers)
		{
			if (APlayerController* PC = LC->GetPlayerController(World))
			{
				if (PC->GetPawn())
					continue;

				return PC;
			}
		}
	}

	return nullptr;
}

void ULocalPlayerBlueprintLibrary::ChangeControlType(APlayerController* InPlayerController, EControlType NewControlType)
{
	if (InPlayerController)
	{
		if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(InPlayerController))
		{
			SFPC->ChangeControlType(NewControlType);
		}
	}
}