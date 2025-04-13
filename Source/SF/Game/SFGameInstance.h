// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/GameInstance.h"
#include "SFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SF_API USFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULocalPlayer* CreateLocalPlayer(APlayerController* NewPlayerController, FString& OutError);


};
