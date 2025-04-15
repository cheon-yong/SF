// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LocalPlayer.h"
#include "SFLocalPlayer.generated.h"

class APlayerCameraManager;

/**
 * 
 */
UCLASS()
class SF_API USFLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:

	USFLocalPlayer();

	virtual bool CalcSceneViewInitOptions(
		struct FSceneViewInitOptions& OutInitOptions,
		FViewport* Viewport,
		class FViewElementDrawer* ViewDrawer = NULL,
		int32 StereoViewIndex = INDEX_NONE) override;

	virtual FSceneView* CalcSceneView(class FSceneViewFamily* ViewFamily,
		FVector& OutViewLocation,
		FRotator& OutViewRotation,
		FViewport* Viewport,
		class FViewElementDrawer* ViewDrawer = NULL,
		int32 StereoViewIndex = INDEX_NONE) override;
protected:
	virtual bool GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex = INDEX_NONE) const override;

protected:
	mutable float LocalPlayerCachedLODDistanceFactor;

public:

	

	TObjectPtr<APlayerCameraManager> PlayerCameraManager;
};
