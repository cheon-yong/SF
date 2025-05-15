// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFLocalPlayer.h"

#include "Camera/CameraComponent.h"
#include "Camera/SFGameViewportClient.h"
#include "SceneView.h"
#include "SceneViewExtension.h"
#include "Player/SFPlayerController.h"
#include <Engine/DebugCameraController.h>

USFLocalPlayer::USFLocalPlayer()
{
}

bool USFLocalPlayer::CalcSceneViewInitOptions(
	FSceneViewInitOptions& ViewInitOptions,
	FViewport* Viewport,
	FViewElementDrawer* ViewDrawer,
	int32 StereoViewIndex)
{
	bool bResult = Super::CalcSceneViewInitOptions(ViewInitOptions, Viewport, ViewDrawer, StereoViewIndex);

	if (!bResult)
	{
		return false;
	}

	if (USFGameViewportClient* SFViewport = Cast< USFGameViewportClient>(GetWorld()->GetGameViewport()))
	{
		if (SFViewport->GetScreenType() == ESFSplitScreenType::OffsetScreen)
		{
			if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(GetPlayerController(GetWorld())))
			{
				float OffsetX = SFViewport->GetOffsetX();
				float ElapsedTime = SFViewport->GetElapedTime();
				float ChangeDuration = SFViewport->GetChangeDuration();

				float Alpha = FMath::Clamp(ElapsedTime / ChangeDuration, 0.f, OffsetX);

				if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
				{
					if (SFPC->bMainController == true)
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] -= Alpha;
					}
					else
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] += Alpha;
					}
				}
				else
				{
					if (SFPC->bMainController == true)
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] += Alpha;
					}
					else
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] -= Alpha;
					}
				}
			}
		}
	}

	return bResult;
}

FSceneView* USFLocalPlayer::CalcSceneView(FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, FViewElementDrawer* ViewDrawer, int32 StereoViewIndex)
{
	FSceneView* OriginView = Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);


	return OriginView;
}