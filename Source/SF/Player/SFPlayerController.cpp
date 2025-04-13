// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFPlayerController.h"
#include "Game/SFGameMode.h"
#include "Game/SFGameInstance.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Character/SFCharacter.h"

UE_DISABLE_OPTIMIZATION
void ASFPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

    if (UGameInstance* SFGameInstance = Cast<UGameInstance>(GetWorld()->GetGameInstance()))
    {
        for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
        {
            if (PS)
            {
                if (PS == PlayerState)
                    continue;

                ASFCharacter* SFCharacter = Cast<ASFCharacter>(PS->GetPawn());
                if (SFCharacter == nullptr)
                    continue;

                FString Error;
                FPlatformUserId UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(PS->PlayerId);
                SFGameInstance->CreateLocalPlayer(UserId, Error, true);
                //SFGameInstance->CreateLocalPlayer(PS->GetPlayerController(), Error);

                GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
                    FString::Printf(TEXT("Local Player Count : %d"), SFGameInstance->GetNumLocalPlayers()));
            }
        }
    }
}
UE_ENABLE_OPTIMIZATION

void ASFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

}