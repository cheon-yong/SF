// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Camera/SFGameViewportClient.h"
#include "ViewportBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class SF_API UViewportBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Viewport")
	static void SetSplitScreenType(ESFSplitScreenType NewSplitScreenType);
};
