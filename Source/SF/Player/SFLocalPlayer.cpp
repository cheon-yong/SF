// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFLocalPlayer.h"

#include "IXRTrackingSystem.h"
#include "IXRCamera.h"
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

	//// 전체 Viewport 크기
	//const FIntPoint FullSize = Viewport->GetSizeXY();
	//const int32 FullWidth = FullSize.X;
	//const int32 FullHeight = FullSize.Y;

	//ViewInitOptions.SetViewRectangle(FIntRect(0, 0, FullWidth / 2, FullHeight));

	//const APlayerController* PC = GetPlayerController(GetWorld());
	//const FMinimalViewInfo& POV = PC->PlayerCameraManager->GetCameraCacheView();

	//float AspectRatio = (float)FullWidth / (float)FullHeight;
	if (USFGameViewportClient* SFViewport = Cast< USFGameViewportClient>(GetWorld()->GetGameViewport()))
	{
		if (SFViewport->GetScreenType() == ESFSplitScreenType::OffsetScreen)
		{
			if (ASFPlayerController* SFPC = Cast<ASFPlayerController>(GetPlayerController(GetWorld())))
			{
				float OffsetX = SFViewport->GetOffsetX();

				if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
				{
					if (SFPC->bMainController == true)
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] -= OffsetX;
					}
					else
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] += OffsetX;
					}
				}
				else
				{
					if (SFPC->bMainController == true)
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] += OffsetX;
					}
					else
					{
						ViewInitOptions.ProjectionMatrix.M[2][0] -= OffsetX;
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

bool USFLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const
{
	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}
