// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SFGameInstance.h"
#include "Player/SFLocalPlayer.h"
#include "Player/SFPlayerController.h"

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
		if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(NewPlayer->PlayerController))
		{
			SFPC->bMainController = false;
			if (SFPC->PlayerCameraManager)
			{
				const FMinimalViewInfo& POV = SFPC->PlayerCameraManager->GetCameraCachePOV();
				
				UE_LOG(LogTemp, Warning, TEXT("=== Camera State ==="));
				UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *POV.Location.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *POV.Rotation.ToString());
				UE_LOG(LogTemp, Warning, TEXT("FOV: %f"), POV.FOV);
				UE_LOG(LogTemp, Warning, TEXT("DefaultAspectRatio: %f"), SFPC->PlayerCameraManager->DefaultAspectRatio);
				UE_LOG(LogTemp, Warning, TEXT("AspectRatio: %f"), POV.AspectRatio);
				UE_LOG(LogTemp, Warning, TEXT("BlendWeight: %f"), POV.PostProcessBlendWeight);
				UE_LOG(LogTemp, Warning, TEXT("=== End Camera State ==="));
			}
		}

		NewPlayer->PlayerController->PlayerCameraManager->DefaultAspectRatio = 1.777778f;
	
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
		
		ASFPlayerController* NewPlayerController = GetWorld()->SpawnActor<ASFPlayerController>();
		NewPlayerController->PlayerCameraManager = PlayerCameraManager;
		NewPlayer->PlayerController = NewPlayerController;
		//NewPlayer->PlayerCameraManager = PlayerCameraManager;

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
				/*TArray<FString> Options;
				NewPlayer->SendSplitJoin(Options);*/
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

USFLocalPlayer* USFGameInstance::CreateLocalPlayer(ASFPlayerState* PlayerState, FString& OutError)
{
	return nullptr;
}

