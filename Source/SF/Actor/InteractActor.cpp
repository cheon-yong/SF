// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Widget/SFWidgetComponent.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;

	WidgetComp = CreateDefaultSubobject<USFWidgetComponent>(TEXT("Widget"));
	WidgetComp->SetupAttachment(RootComponent);
}

void AInteractActor::ShowInteractWidget(bool bOn)
{
	WidgetComp->SetVisibility(bOn);
}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
	WidgetComp->SetVisibility(false);
}

