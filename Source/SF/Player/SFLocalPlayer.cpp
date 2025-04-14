// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFLocalPlayer.h"

bool USFLocalPlayer::CalcSceneViewInitOptions(
	FSceneViewInitOptions& ViewInitOptions, 
	FViewport* Viewport, 
	FViewElementDrawer* ViewDrawer, 
	int32 StereoViewIndex)
{
	return Super::CalcSceneViewInitOptions(ViewInitOptions, Viewport, ViewDrawer, StereoViewIndex);
}

FSceneView* USFLocalPlayer::CalcSceneView(FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, FViewElementDrawer* ViewDrawer, int32 StereoViewIndex)
{
	return Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);
}
