// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SFAnimNotify_Projectile.h"
#include "Character/SFNonPlayerCharacter.h"

void USFAnimNotify_Projectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		ASFNonPlayerCharacter* NPC = Cast<ASFNonPlayerCharacter>(MeshComp->GetOwner());
		if (NPC)
		{
			NPC->CreateProjectile();
		}
	}
}
