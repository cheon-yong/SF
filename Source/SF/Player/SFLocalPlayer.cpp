// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFLocalPlayer.h"

#include "IXRTrackingSystem.h"
#include "IXRCamera.h"
#include "Camera/CameraComponent.h"
#include "SceneView.h"
#include "SceneViewExtension.h"
#include <Engine/DebugCameraController.h>

#if !UE_BUILD_SHIPPING

static TAutoConsoleVariable<int32> CVarViewportTest(
	TEXT("r.Test.ConstrainedView"),
	0,
	TEXT("Allows to test different viewport rectangle configuations (in game only) as they can happen when using cinematics/Editor.\n")
	TEXT("0: off(default)\n")
	TEXT("1..7: Various Configuations"),
	ECVF_RenderThreadSafe);

#endif // !UE_BUILD_SHIPPING

int32 GCalcLocalPlayerCachedLODDistanceFactor = 1;
static FAutoConsoleVariableRef CVarCalcLocalPlayerCachedLODDistanceFactor(
	TEXT("r.CalcLocalPlayerCachedLODDistanceFactor"),
	GCalcLocalPlayerCachedLODDistanceFactor,
	TEXT("Should we calculate a LOD Distance Factor based on the current FOV.  Should not be necessary since LOD is already based on screen size.\n")
);

DECLARE_CYCLE_STAT(TEXT("CalcSceneView"), STAT_CalcSceneView, STATGROUP_Engine);


USFLocalPlayer::USFLocalPlayer()
{
	LocalPlayerCachedLODDistanceFactor = 1.0f;
}

bool USFLocalPlayer::CalcSceneViewInitOptions(
	FSceneViewInitOptions& ViewInitOptions,
	FViewport* Viewport,
	FViewElementDrawer* ViewDrawer,
	int32 StereoViewIndex)
{

	return Super::CalcSceneViewInitOptions(ViewInitOptions, Viewport, ViewDrawer, StereoViewIndex);

	//QUICK_SCOPE_CYCLE_COUNTER(STAT_CalcSceneViewInitOptions);
	//if ((Size.X <= 0.f) || (Size.Y <= 0.f) || (Viewport == NULL))
	//{
	//	return false;
	//}
	//// get the projection data
	//if (GetProjectionData(Viewport, /*inout*/ ViewInitOptions, StereoViewIndex) == false)
	//{
	//	// Return NULL if tshis we didn't get back the info we needed
	//	return false;
	//}

	//// return if we have an invalid view rect
	//if (!ViewInitOptions.IsValidViewRectangle())
	//{
	//	return false;
	//}

	//if (PlayerController != nullptr)
	//	PlayerCameraManager = PlayerController->PlayerCameraManager;

	//if (PlayerCameraManager != NULL)
	//{
	//	// Apply screen fade effect to screen.
	//	if (PlayerCameraManager->bEnableFading)
	//	{
	//		ViewInitOptions.OverlayColor = PlayerCameraManager->FadeColor;
	//		ViewInitOptions.OverlayColor.A = FMath::Clamp(PlayerCameraManager->FadeAmount, 0.0f, 1.0f);
	//	}

	//	// Do color scaling if desired.
	//	if (PlayerCameraManager->bEnableColorScaling)
	//	{
	//		ViewInitOptions.ColorScale = FLinearColor(
	//			PlayerCameraManager->ColorScale.X,
	//			PlayerCameraManager->ColorScale.Y,
	//			PlayerCameraManager->ColorScale.Z
	//		);
	//	}

	//	// Was there a camera cut this frame?
	//	ViewInitOptions.bInCameraCut = PlayerCameraManager->bGameCameraCutThisFrame;
	//}

	//if (GEngine->IsStereoscopic3D(Viewport))
	//{
	//	ViewInitOptions.StereoPass = GEngine->StereoRenderingDevice->GetViewPassForIndex(StereoViewIndex != INDEX_NONE, StereoViewIndex);
	//}

	//if (PlayerCameraManager == nullptr)
	//	return false;

	//check(PlayerCameraManager);

	//const uint32 ViewIndex = StereoViewIndex != INDEX_NONE ? StereoViewIndex : 0;

	//// Make sure the ViewStates array has enough elements for the given ViewIndex.
	//{
	//	const int32 RequiredViewStates = (ViewIndex + 1) - ViewStates.Num();

	//	if (RequiredViewStates > 0)
	//	{
	//		ViewStates.AddDefaulted(RequiredViewStates);
	//	}
	//}

	//// Allocate the current ViewState if necessary
	//if (ViewStates[ViewIndex].GetReference() == nullptr)
	//{
	//	const UWorld* CurrentWorld = GetWorld();
	//	const ERHIFeatureLevel::Type FeatureLevel = CurrentWorld ? CurrentWorld->GetFeatureLevel() : GMaxRHIFeatureLevel;

	//	ViewStates[ViewIndex].Allocate(FeatureLevel);
	//}

	//ViewInitOptions.SceneViewStateInterface = ViewStates[ViewIndex].GetReference();
	//ViewInitOptions.ViewActor = PlayerCameraManager->GetViewTarget();

	//// TODO: Switch to GetLocalPlayerIndex during GetControllerId deprecation, this is only used by MotionControllerComponent
	//ViewInitOptions.PlayerIndex = GetControllerId();
	//ViewInitOptions.ViewElementDrawer = ViewDrawer;
	//ViewInitOptions.BackgroundColor = FLinearColor::Black;
	//ViewInitOptions.LODDistanceFactor = PlayerController ? PlayerController->LocalPlayerCachedLODDistanceFactor : 1.0f;
	//ViewInitOptions.StereoViewIndex = StereoViewIndex;
	//ViewInitOptions.WorldToMetersScale = PlayerController ? PlayerController->GetWorldSettings()->WorldToMeters : GetWorld()->GetWorldSettings()->WorldToMeters;
	//ViewInitOptions.CursorPos = Viewport->HasMouseCapture() ? FIntPoint(-1, -1) : FIntPoint(Viewport->GetMouseX(), Viewport->GetMouseY());
	//ViewInitOptions.OriginOffsetThisFrame = PlayerController ? PlayerController->GetWorld()->OriginOffsetThisFrame : GetWorld()->OriginOffsetThisFrame;

	//return true;

	//return Super::CalcSceneViewInitOptions(ViewInitOptions, Viewport, ViewDrawer, StereoViewIndex);
}

FSceneView* USFLocalPlayer::CalcSceneView(FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, FViewElementDrawer* ViewDrawer, int32 StereoViewIndex)
{
	return Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);

//	SCOPE_CYCLE_COUNTER(STAT_CalcSceneView);
//
//	FSceneViewInitOptions ViewInitOptions;
//
//	if (!CalcSceneViewInitOptions(ViewInitOptions, Viewport, ViewDrawer, StereoViewIndex))
//	{
//		return nullptr;
//	}
//
//	// Get the viewpoint...technically doing this twice
//	// but it makes GetProjectionData better
//	FMinimalViewInfo ViewInfo;
//	GetViewPoint(ViewInfo);
//	ViewInitOptions.ViewLocation = ViewInfo.Location;
//	ViewInitOptions.ViewRotation = ViewInfo.Rotation;
//	ViewInitOptions.bUseFieldOfViewForLOD = ViewInfo.bUseFieldOfViewForLOD;
//	ViewInitOptions.FOV = ViewInfo.FOV;
//	ViewInitOptions.DesiredFOV = ViewInfo.DesiredFOV;
//
//	// Fill out the rest of the view init options
//	ViewInitOptions.ViewFamily = ViewFamily;
//
//	if (PlayerController && !PlayerController->bRenderPrimitiveComponents)
//	{
//		// Emplaces an empty show only primitive list.
//		ViewInitOptions.ShowOnlyPrimitives.Emplace();
//	}
//	else
//	{
//		QUICK_SCOPE_CYCLE_COUNTER(STAT_BuildHiddenComponentList);
//		PlayerController->BuildHiddenComponentList(ViewInfo.Location, /*out*/ ViewInitOptions.HiddenPrimitives);
//	}
//
//	//@TODO: SPLITSCREEN: This call will have an issue with splitscreen, as the show flags are shared across the view family
//	EngineShowFlagOrthographicOverride(ViewInitOptions.IsPerspectiveProjection(), ViewFamily->EngineShowFlags);
//
//	FSceneView* const View = new FSceneView(ViewInitOptions);
//
//	OutViewLocation = View->ViewLocation;
//	OutViewRotation = View->ViewRotation;
//	// Pass on the previous view transform from the view info (probably provided by the camera if set)
//	View->PreviousViewTransform = ViewInfo.PreviousViewTransform;
//
//	ViewFamily->Views.Add(View);
//
//	{
//		QUICK_SCOPE_CYCLE_COUNTER(STAT_PostprocessSettings);
//		CSV_SCOPED_TIMING_STAT_EXCLUSIVE(PostProcessSettings);
//		View->StartFinalPostprocessSettings(ViewInfo.Location);
//
//		TArray<FPostProcessSettings> const* CameraAnimPPSettings = nullptr;
//		TArray<float> const* CameraAnimPPBlendWeights = nullptr;
//		TArray<EViewTargetBlendOrder> const* CameraAnimPPBlendOrders = nullptr;
//
//		// Base overrides (post process volumes, etc)
//		if (PlayerController->PlayerCameraManager)
//		{
//			PlayerController->PlayerCameraManager->GetCachedPostProcessBlends(CameraAnimPPSettings, CameraAnimPPBlendWeights, CameraAnimPPBlendOrders);
//
//			for (int32 PPIdx = 0; PPIdx < CameraAnimPPBlendWeights->Num(); ++PPIdx)
//			{
//				if ((*CameraAnimPPBlendOrders)[PPIdx] == VTBlendOrder_Base)
//				{
//					View->OverridePostProcessSettings((*CameraAnimPPSettings)[PPIdx], (*CameraAnimPPBlendWeights)[PPIdx]);
//				}
//			}
//		}
//
//		// Main camera
//		View->OverridePostProcessSettings(ViewInfo.PostProcessSettings, ViewInfo.PostProcessBlendWeight);
//
//		// Camera overrides (cameras blending in, camera modifiers, etc)
//		if (PlayerController->PlayerCameraManager)
//		{
//			checkSlow(CameraAnimPPBlendWeights && CameraAnimPPBlendOrders && CameraAnimPPSettings);
//			for (int32 PPIdx = 0; PPIdx < CameraAnimPPBlendWeights->Num(); ++PPIdx)
//			{
//				if ((*CameraAnimPPBlendOrders)[PPIdx] == VTBlendOrder_Override)
//				{
//					View->OverridePostProcessSettings((*CameraAnimPPSettings)[PPIdx], (*CameraAnimPPBlendWeights)[PPIdx]);
//				}
//			}
//
//			PlayerController->PlayerCameraManager->UpdatePhotographyPostProcessing(View->FinalPostProcessSettings);
//		}
//
//		if (GEngine->IsStereoscopic3D(Viewport))
//		{
//			FPostProcessSettings StereoDeviceOverridePostProcessinSettings;
//			float BlendWeight = 1.0f;
//			bool StereoSettingsAvailable = GEngine->StereoRenderingDevice->OverrideFinalPostprocessSettings(&StereoDeviceOverridePostProcessinSettings, View->StereoPass, View->StereoViewIndex, BlendWeight);
//			if (StereoSettingsAvailable)
//			{
//				View->OverridePostProcessSettings(StereoDeviceOverridePostProcessinSettings, BlendWeight);
//			}
//		}
//
//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//		ADebugCameraController* DebugCameraController = Cast<ADebugCameraController>(PlayerController);
//		if (DebugCameraController != nullptr)
//		{
//			DebugCameraController->UpdateVisualizeBufferPostProcessing(View->FinalPostProcessSettings);
//		}
//#endif
//
//		View->EndFinalPostprocessSettings(ViewInitOptions);
//	}
//
//	for (int ViewExt = 0; ViewExt < ViewFamily->ViewExtensions.Num(); ViewExt++)
//	{
//		ViewFamily->ViewExtensions[ViewExt]->SetupView(*ViewFamily, *View);
//	}
//
//	return View;

	//return Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);
}

bool USFLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const
{
	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
//
//	// If the actor
//	if ((Viewport == NULL) || (Viewport->GetSizeXY().X == 0) || (Viewport->GetSizeXY().Y == 0) || (Size.X == 0) || (Size.Y == 0))
//	{
//		return false;
//	}
//
//	int32 X = FMath::TruncToInt(Origin.X * Viewport->GetSizeXY().X);
//	int32 Y = FMath::TruncToInt(Origin.Y * Viewport->GetSizeXY().Y);
//
//	X += Viewport->GetInitialPositionXY().X;
//	Y += Viewport->GetInitialPositionXY().Y;
//
//	uint32 SizeX = FMath::TruncToInt(Size.X * Viewport->GetSizeXY().X);
//	uint32 SizeY = FMath::TruncToInt(Size.Y * Viewport->GetSizeXY().Y);
//
//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//
//	// We expect some size to avoid problems with the view rect manipulation
//	if (SizeX > 50 && SizeY > 50)
//	{
//		int32 Value = CVarViewportTest.GetValueOnGameThread();
//
//		if (Value)
//		{
//			int InsetX = SizeX / 4;
//			int InsetY = SizeY / 4;
//
//			// this allows to test various typical view port situations (todo: split screen)
//			switch (Value)
//			{
//			case 1: X += InsetX; Y += InsetY; SizeX -= InsetX * 2; SizeY -= InsetY * 2; break;
//			case 2: Y += InsetY; SizeY -= InsetY * 2; break;
//			case 3: X += InsetX; SizeX -= InsetX * 2; break;
//			case 4: SizeX /= 2; SizeY /= 2; break;
//			case 5: SizeX /= 2; SizeY /= 2; X += SizeX;	break;
//			case 6: SizeX /= 2; SizeY /= 2; Y += SizeY; break;
//			case 7: SizeX /= 2; SizeY /= 2; X += SizeX; Y += SizeY; break;
//			}
//		}
//	}
//#endif
//
//	FIntRect UnconstrainedRectangle = FIntRect(X, Y, X + SizeX, Y + SizeY);
//
//	ProjectionData.SetViewRectangle(UnconstrainedRectangle);
//
//	// Get the viewpoint.
//	FMinimalViewInfo ViewInfo;
//	GetViewPoint(/*out*/ ViewInfo);
//
//	// If stereo rendering is enabled, update the size and offset appropriately for this pass
//	const bool bNeedStereo = StereoViewIndex != INDEX_NONE && GEngine->IsStereoscopic3D();
//	const bool bIsHeadTrackingAllowed =
//		GEngine->XRSystem.IsValid() &&
//		(GetWorld() != nullptr ? GEngine->XRSystem->IsHeadTrackingAllowedForWorld(*GetWorld()) : GEngine->XRSystem->IsHeadTrackingAllowed());
//	if (bNeedStereo)
//	{
//		GEngine->StereoRenderingDevice->AdjustViewRect(StereoViewIndex, X, Y, SizeX, SizeY);
//	}
//
//	// scale distances for cull distance purposes by the ratio of our current FOV to the default FOV
//	if (GCalcLocalPlayerCachedLODDistanceFactor != 0)
//	{
//		LocalPlayerCachedLODDistanceFactor = ViewInfo.FOV / FMath::Max<float>(0.01f, (PlayerCameraManager != NULL) ? PlayerCameraManager->DefaultFOV : 90.f);
//	}
//	else // This should be removed in the final version. Leaving in so this can be toggled on and off in order to evaluate it.
//	{
//		LocalPlayerCachedLODDistanceFactor = 1.f;
//	}
//
//	FVector StereoViewLocation = ViewInfo.Location;
//	if (bNeedStereo || bIsHeadTrackingAllowed)
//	{
//		auto XRCamera = GEngine->XRSystem.IsValid() ? GEngine->XRSystem->GetXRCamera() : nullptr;
//		if (XRCamera.IsValid())
//		{
//			AActor* ViewTarget = PlayerController->GetViewTarget();
//			const bool bHasActiveCamera = ViewTarget && ViewTarget->HasActiveCameraComponent();
//			XRCamera->UseImplicitHMDPosition(bHasActiveCamera);
//		}
//
//		if (GEngine->IsStereoscopic3D(Viewport))
//		{
//			GEngine->StereoRenderingDevice->CalculateStereoViewOffset(StereoViewIndex, ViewInfo.Rotation, GetWorld()->GetWorldSettings()->WorldToMeters, StereoViewLocation);
//		}
//	}
//
//	// Create the view matrix
//	ProjectionData.ViewOrigin = StereoViewLocation;
//	ProjectionData.ViewRotationMatrix = FInverseRotationMatrix(ViewInfo.Rotation) * FMatrix(
//		FPlane(0, 0, 1, 0),
//		FPlane(1, 0, 0, 0),
//		FPlane(0, 1, 0, 0),
//		FPlane(0, 0, 0, 1));
//
//	// @todo viewext this use case needs to be revisited
//	if (!bNeedStereo)
//	{
//		// Create the projection matrix (and possibly constrain the view rectangle)
//		FMinimalViewInfo::CalculateProjectionMatrixGivenView(ViewInfo, AspectRatioAxisConstraint, Viewport, /*inout*/ ProjectionData);
//
//		for (auto& ViewExt : GEngine->ViewExtensions->GatherActiveExtensions(FSceneViewExtensionContext(Viewport)))
//		{
//			ViewExt->SetupViewProjectionMatrix(ProjectionData);
//		};
//	}
//	else
//	{
//		// Let the stereoscopic rendering device handle creating its own projection matrix, as needed
//		ProjectionData.ProjectionMatrix = GEngine->StereoRenderingDevice->GetStereoProjectionMatrix(StereoViewIndex);
//
//		// calculate the out rect
//		ProjectionData.SetViewRectangle(FIntRect(X, Y, X + SizeX, Y + SizeY));
//	}
//
//	return true;
}
