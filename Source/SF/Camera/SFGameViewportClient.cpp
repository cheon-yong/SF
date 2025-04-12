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
	SFSplitscreenInfo[ESFSplitScreenType::OneScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.0f, 0.0f, 0.0f, 0.0f));

	SFSplitscreenInfo[ESFSplitScreenType::SeparateScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.0f, 0.0f));
	SFSplitscreenInfo[ESFSplitScreenType::SeparateScreen].PlayerData.Add(FPerPlayerSplitscreenData(0.4999f, 1.0f, 0.5001f, 0.0f));

	SplitScreenType = ESFSplitScreenType::OneScreen;
}

void USFGameViewportClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateScreenInfo();
}

void USFGameViewportClient::SetSplitScreenType(ESFSplitScreenType NewSplitScreenType)
{

}

void USFGameViewportClient::UpdateScreenInfo()
{

}
