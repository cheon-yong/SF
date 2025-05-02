// Fill out your copyright notice in the Description page of Project Settings.


#include "Editor/Actor/LevelOrganizeVolume.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"

ALevelOrganizeVolume::ALevelOrganizeVolume()
{
    PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetCollisionProfileName(TEXT("EditorOnly"));
#endif
}

#if WITH_EDITOR
void ALevelOrganizeVolume::MoveOverlappingActorsToFolder()
{
    UWorld* World = GetWorld();
    if (!World) return;

    const FBox Bounds = BoxComponent->Bounds.GetBox();

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor == this) continue;

        if (Actor->HiddenEditorViews)
            continue;

        FName ActorFolderPath = Actor->GetFolderPath();
        if (ActorFolderPath != FFolder::GetEmptyPath())
        {
            continue;
        }

        if (Actor->IsHiddenEd())
        {
            continue;
        }

        if (Bounds.IsInside(Actor->GetActorLocation()))
        {
            Actor->SetFolderPath(*TargetFolder);
            UE_LOG(LogTemp, Log, TEXT("Moved %s to folder %s"), *Actor->GetName(), *TargetFolder);
        }
    }
}
#endif