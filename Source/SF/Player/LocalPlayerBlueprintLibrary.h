// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include <Input/SFInputHandler.h>
#include "LocalPlayerBlueprintLibrary.generated.h"


class APlayerController;
/**
 * 
 */
UCLASS()
class SF_API ULocalPlayerBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	static APlayerController* GetServerControllerInClient(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	static void ChangeControlType(APlayerController* PlayerController, EControlType NewControlType);
};
