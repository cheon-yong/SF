// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SFGameViewportClient.h"

USFGameViewportClient::USFGameViewportClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Test ÄÚµå
	SplitscreenInfo[ESplitScreenType::TwoPlayer_Vertical].PlayerData[0] = FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.0f, 0.0f);
	SplitscreenInfo[ESplitScreenType::TwoPlayer_Vertical].PlayerData[1] = FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.5001f, 0.0f);

	SFSplitscreenInfo.Init(FSplitscreenData(), ESFSplitScreenType::SplitTypeCount);

	SFSplitscreenInfo[ESFSplitScreenType::OneScreen].PlayerData.Add(FPerPlayerSplitscreenData(1.0f, 1.0f, 0.0f, 0.0f));
	SFSplitscreenInfo[ESFSplitScreenType::OneScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.0f, 1.0f, 1.0f, 0.0f));

	SFSplitscreenInfo[ESFSplitScreenType::SeparateScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.0f, 0.0f));
	SFSplitscreenInfo[ESFSplitScreenType::SeparateScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.5001f, 0.0f));

	SplitscreenType = ESFSplitScreenType::SeparateScreen;

	ElapsedTime = 0.f;
	
	ChangeDuration = 1.0f; // Default 1

	bChangingScreenSize = false;
}

void USFGameViewportClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void USFGameViewportClient::LayoutPlayers()
{
	//Super::LayoutPlayers();

	UpdateActiveSplitscreenType();

	UpdateScreenInfo(FApp::GetDeltaTime());
}

void USFGameViewportClient::SetSplitScreenType(ESFSplitScreenType NewSplitScreenType)
{
	if (SplitscreenType == NewSplitScreenType)
		return;

	BeforeSplitscreenType = SplitscreenType;
	SplitscreenType = NewSplitScreenType;
	bChangingScreenSize = true;
	ElapsedTime = 0;
}

void USFGameViewportClient::UpdateScreenInfo(float DeltaTime)
{
	TArray<FPerPlayerSplitscreenData> TempData;

	if (bChangingScreenSize == true)
	{
		ElapsedTime += DeltaTime;
		
		float Alpha = FMath::Clamp(ElapsedTime / ChangeDuration, 0.f, 1.f); // ÁøÇà·ü

		FPerPlayerSplitscreenData NewPlayer1Data = FPerPlayerSplitscreenData(
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[0].SizeX, SFSplitscreenInfo[SplitscreenType].PlayerData[0].SizeX, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[0].SizeY, SFSplitscreenInfo[SplitscreenType].PlayerData[0].SizeY, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[0].OriginX, SFSplitscreenInfo[SplitscreenType].PlayerData[0].OriginX, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[0].OriginY, SFSplitscreenInfo[SplitscreenType].PlayerData[0].OriginY, Alpha)
		);
		TempData.Add(NewPlayer1Data);

		FPerPlayerSplitscreenData NewPlayer2Data = FPerPlayerSplitscreenData(
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[1].SizeX, SFSplitscreenInfo[SplitscreenType].PlayerData[1].SizeX, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[1].SizeY, SFSplitscreenInfo[SplitscreenType].PlayerData[1].SizeY, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[1].OriginX, SFSplitscreenInfo[SplitscreenType].PlayerData[1].OriginX, Alpha),
			FMath::Lerp(SFSplitscreenInfo[BeforeSplitscreenType].PlayerData[1].OriginY, SFSplitscreenInfo[SplitscreenType].PlayerData[1].OriginY, Alpha)
		);
		TempData.Add(NewPlayer2Data);

		if (Alpha >= 1.0f)
		{
			bChangingScreenSize = false;
		}
	}
	else
	{
		FPerPlayerSplitscreenData NewPlayer1Data = SFSplitscreenInfo[SplitscreenType].PlayerData[0];
		TempData.Add(NewPlayer1Data);

		FPerPlayerSplitscreenData NewPlayer2Data = SFSplitscreenInfo[SplitscreenType].PlayerData[1];
		TempData.Add(NewPlayer2Data);
	}

	const TArray<ULocalPlayer*>& PlayerList = GetOuterUEngine()->GetGamePlayers(this);

	for (int32 PlayerIdx = 0; PlayerIdx < PlayerList.Num(); PlayerIdx++)
	{
		PlayerList[PlayerIdx]->Size.X = TempData[PlayerIdx].SizeX;
		PlayerList[PlayerIdx]->Size.Y = TempData[PlayerIdx].SizeY;
		PlayerList[PlayerIdx]->Origin.X = TempData[PlayerIdx].OriginX;
		PlayerList[PlayerIdx]->Origin.Y = TempData[PlayerIdx].OriginY;
	}
}
