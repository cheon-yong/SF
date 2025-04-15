// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SFPlayerController.generated.h"

class USFLocalPlayer;

/**
 * 
 */
UCLASS()
class SF_API ASFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASFPlayerController();

	virtual void OnRep_PlayerState() override;

	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	UFUNCTION(Server, Reliable)
	void Server_DeleteSecondPawn();

	UFUNCTION(Client, Reliable)
	void Client_UpdateSecondController();
};
