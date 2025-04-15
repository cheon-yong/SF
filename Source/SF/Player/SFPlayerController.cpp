// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFPlayerController.h"
#include "Game/SFGameMode.h"
#include "Game/SFGameInstance.h"
#include "Player/SFPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/SFCharacter.h"
#include "Player/SFLocalPlayer.h"

UE_DISABLE_OPTIMIZATION
ASFPlayerController::ASFPlayerController()
{
	
}
void ASFPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			if (GameInstance->GetNumLocalPlayers() < 2)
			{
				FString Error;
				int32 NewId = GameInstance->GetLocalPlayerByIndex(0)->GetControllerId() + 1;
				GameInstance->CreateLocalPlayer(NewId, Error, true);
			}
			else
			{
				Server_DeleteSecondPawn();
			}
		}
	}
}

void ASFPlayerController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);

}

UE_DISABLE_OPTIMIZATION
void ASFPlayerController::Client_UpdateSecondController_Implementation()
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		auto LocalPlayers = GameInstance->GetLocalPlayers();
		for (const ULocalPlayer* LC : LocalPlayers)
		{
			if (APlayerController* PC = LC->GetPlayerController(GetWorld()))
			{
				if (PC->GetPawn())
					continue;	

				PC->bAutoManageActiveCameraTarget = false;	
			}

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
				FString::Printf(TEXT("Controller : %s"), *LC->GetPlayerController(GetWorld())->GetViewTarget()->GetName()));

		}
	}
}
UE_ENABLE_OPTIMIZATION


void ASFPlayerController::Server_DeleteSecondPawn_Implementation()
{
	AActor* NewViewTarget = nullptr;
	for (FConstPlayerControllerIterator PCIt = GetWorld()->GetPlayerControllerIterator(); PCIt; ++PCIt)
	{
		APlayerController* PC = PCIt->Get();
		if (PC->IsLocalController())
		{
			NewViewTarget = PC->GetViewTarget();
			continue;
		}
		
		if (PCIt.GetIndex() == GetWorld()->GetNumPlayerControllers() - 1)
		{
			//PCIt->Get()->UnPossess();
			APawn* NewPawn = PC->GetPawn();
			PC->UnPossess();
			GetWorld()->DestroyActor(NewPawn);
			PC->SetViewTarget(NewViewTarget);
			
			Client_UpdateSecondController();
		}
	}
}
UE_ENABLE_OPTIMIZATION
