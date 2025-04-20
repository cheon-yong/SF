// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SFGameMode.generated.h"

UCLASS(minimalapi)
class ASFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASFGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

};



