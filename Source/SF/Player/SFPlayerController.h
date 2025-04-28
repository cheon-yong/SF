// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/SFInputHandler.h"
#include "GameFramework/PlayerController.h"
#include "SFPlayerController.generated.h"

class USFLocalPlayer;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


/**
 * 
 */
UCLASS()
class SF_API ASFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASFPlayerController();

	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void ChangeControlType(EControlType NewControlType);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ChangeControlType(EControlType NewControlType);

	UFUNCTION(Server, Reliable)
	void Server_DeleteSecondPawn();

	UFUNCTION(Client, Reliable)
	void Client_UpdateSecondController();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ShowMouseCurser(bool bShow);

protected:
	void BindInputHandler(USFInputHandler* InputHandler);


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<EControlType, TSubclassOf<USFInputHandler>> InputHandlerClassMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	EControlType DefaultType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	USFInputHandler* CurrentInputHandler;

	bool bMainController = true;
};
