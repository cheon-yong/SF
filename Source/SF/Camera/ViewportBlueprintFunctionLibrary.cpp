// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ViewportBlueprintFunctionLibrary.h"

void UViewportBlueprintFunctionLibrary::SetSplitScreenType(ESFSplitScreenType NewSplitScreenType)
{
	if (GEngine && GEngine->GameViewport)
	{
		if (USFGameViewportClient* SFGameViewportClient = Cast<USFGameViewportClient>(GEngine->GameViewport))
		{
			SFGameViewportClient->SetSplitScreenType(NewSplitScreenType);
		}
	}
}
