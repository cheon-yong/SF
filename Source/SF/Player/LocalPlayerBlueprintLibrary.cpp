// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LocalPlayerBlueprintLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/GameInstance.h"
#include "Player/SFPlayerController.h"
#include "Player/SFLocalPlayer.h"

UE_DISABLE_OPTIMIZATION
APlayerController* ULocalPlayerBlueprintLibrary::GetServerControllerInClient(const UObject* WorldContext)
{

	if (WorldContext == nullptr)
		return nullptr;

	UWorld* World = WorldContext->GetWorld();

	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (World->GetNetMode() != ENetMode::NM_Client)
		{	
			return nullptr;
		}

		auto LocalPlayers = GameInstance->GetLocalPlayers();
		for (const ULocalPlayer* LC : LocalPlayers)
		{
			if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(LC->GetPlayerController(World)))
			{
				if (SFPC->bMainController == false)
				{
					return SFPC;
				}
			}

			/*if (APlayerController* PC = LC->GetPlayerController(World))
			{
				if (PC->GetPawn())
					continue;

				return PC;
			}*/
		}
	}

	return nullptr;
}
ULocalPlayer* ULocalPlayerBlueprintLibrary::GetSecondLocalPlayerInServer(const UObject* WorldContext)
{
	if (WorldContext == nullptr)
		return nullptr;

	UWorld* World = WorldContext->GetWorld();
	
	int index = 0;
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (index == 0)
		{
			index++;
			continue;
		}
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
			if (LocalPlayer)
			{
				return LocalPlayer;
			}
		}
	}

	return nullptr;
}
UE_ENABLE_OPTIMIZATION

void ULocalPlayerBlueprintLibrary::ChangeControlType(APlayerController* InPlayerController, EControlType NewControlType)
{
	if (InPlayerController)
	{
		if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(InPlayerController))
		{
			if (SFPC->HasAuthority())
				SFPC->Multicast_ChangeControlType(NewControlType);
		}
	}
}