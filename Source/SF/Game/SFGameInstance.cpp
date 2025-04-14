// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SFGameInstance.h"
#include "Player/SFLocalPlayer.h"

ULocalPlayer* USFGameInstance::CreateLocalPlayer(APlayerController* NewPlayerController, FString& OutError)
{
	check(GetEngine()->LocalPlayerClass != NULL);
	check(NewPlayerController);

	ULocalPlayer* NewPlayer = NULL;
	int32 InsertIndex = INDEX_NONE;
	UGameViewportClient* GameViewport = GetGameViewportClient();

	if (GameViewport == nullptr)
	{
		if (ensure(IsDedicatedServerInstance()))
		{
			OutError = FString::Printf(TEXT("Dedicated servers cannot have local players"));
			return nullptr;
		}
	}

	const int32 MaxSplitscreenPlayers = GameViewport ? GameViewport->MaxSplitscreenPlayers : 1;


	FPlatformUserId UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(0);
	if (NewPlayerController->GetNetConnection())
		UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(NewPlayerController->GetNetConnection()->GetConnectionId());

	if (FindLocalPlayerFromPlatformUserId(UserId) != NULL)
	{

	}
	else if (LocalPlayers.Num() < MaxSplitscreenPlayers)
	{
		NewPlayer = NewObject<ULocalPlayer>(GetEngine(), GetEngine()->LocalPlayerClass);
		
		NewPlayer->PlayerController = NewPlayerController;

		InsertIndex = AddLocalPlayer(NewPlayer, UserId);
		UWorld* CurrentWorld = GetWorld();
		if (InsertIndex != INDEX_NONE && CurrentWorld != nullptr)
		{
			if (CurrentWorld->GetNetMode() != NM_Client)
			{
				
			}
			else
			{
				// client; ask the server to let the new player join
				TArray<FString> Options;
				NewPlayer->SendSplitJoin(Options);
			}
		}
	}
	else
	{
		OutError = FString::Printf(TEXT("Maximum number of players (%d) already created.  Unable to create more."), MaxSplitscreenPlayers);
	}

	if (OutError != TEXT(""))
	{
		UE_LOG(LogPlayerManagement, Log, TEXT("UPlayer* creation failed with error: %s"), *OutError);
	}

	return NewPlayer;
}

USFLocalPlayer* USFGameInstance::CreateLocalPlayer(APlayerCameraManager* PlayerCameraManager, FString& OutError)
{
	check(GetEngine()->LocalPlayerClass != NULL);
	check(PlayerCameraManager);

	USFLocalPlayer* NewPlayer = NULL;
	int32 InsertIndex = INDEX_NONE;
	UGameViewportClient* GameViewport = GetGameViewportClient();

	if (GameViewport == nullptr)
	{
		if (ensure(IsDedicatedServerInstance()))
		{
			OutError = FString::Printf(TEXT("Dedicated servers cannot have local players"));
			return nullptr;
		}
	}

	const int32 MaxSplitscreenPlayers = GameViewport ? GameViewport->MaxSplitscreenPlayers : 1;


	FPlatformUserId UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(-1);

	if (FindLocalPlayerFromPlatformUserId(UserId) != NULL)
	{

	}
	else if (LocalPlayers.Num() < MaxSplitscreenPlayers)
	{
		NewPlayer = NewObject<USFLocalPlayer>(GetEngine(), GetEngine()->LocalPlayerClass);

		NewPlayer->PlayerCameraManager = PlayerCameraManager;

		InsertIndex = AddLocalPlayer(NewPlayer, UserId);
		UWorld* CurrentWorld = GetWorld();
		if (InsertIndex != INDEX_NONE && CurrentWorld != nullptr)
		{
			if (CurrentWorld->GetNetMode() != NM_Client)
			{

			}
			else
			{
				// client; ask the server to let the new player join
				TArray<FString> Options;
				NewPlayer->SendSplitJoin(Options);
			}
		}
	}
	else
	{
		OutError = FString::Printf(TEXT("Maximum number of players (%d) already created.  Unable to create more."), MaxSplitscreenPlayers);
	}

	if (OutError != TEXT(""))
	{
		UE_LOG(LogPlayerManagement, Log, TEXT("UPlayer* creation failed with error: %s"), *OutError);
	}

	return NewPlayer;
}

