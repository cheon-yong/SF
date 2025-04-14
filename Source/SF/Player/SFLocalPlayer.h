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

public:
	TObjectPtr<APlayerCameraManager> PlayerCameraManager;
};
