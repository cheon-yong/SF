// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

class UStaticMeshComponent;
class USFWidgetComponent;

UCLASS()
class AInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActor();

	UFUNCTION(BlueprintCallable)
	void ShowInteractWidget(bool bOn);
		
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginInteract(AActor* InInteractPawn);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EndInteract(AActor* InInteractPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USFWidgetComponent> WidgetComp;
};
