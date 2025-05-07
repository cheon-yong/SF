// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "SFGameViewportClient.generated.h"

struct FPerPlayerSplitscreenData;

UENUM(BlueprintType)
enum ESFSplitScreenType : int
{
	None,
	Cinematic,
	OneScreen,
	SeparateScreen,
	OffsetScreen,
	SplitTypeCount
};

/**
 * 
 */
UCLASS()
class SF_API USFGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	USFGameViewportClient(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void LayoutPlayers() override;
	
	virtual void SwapCameraPosition();

	float GetOffsetX() { return OffsetX; }

	ESFSplitScreenType GetScreenType() { return SplitscreenType; }

	void SetSplitScreenType(ESFSplitScreenType NewSplitScreenType);

protected:
	void UpdateScreenInfo(float DeltaTime);

protected:

	
	float ElapsedTime;

	float ChangeDuration;

	bool bChangingScreenSize;

	float OffsetX = 1.f;

	UPROPERTY()
	TEnumAsByte<ESFSplitScreenType> SplitscreenType;

	TEnumAsByte<ESFSplitScreenType> BeforeSplitscreenType;

	TArray<struct FSplitscreenData> SFSplitscreenInfo;
};
