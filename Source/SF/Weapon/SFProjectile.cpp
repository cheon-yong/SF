// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SFProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Character/SFCharacter.h"

// Sets default values
ASFProjectile::ASFProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASFProjectile::OnBeginHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = false;
	SetReplicateMovement(false);
}

void ASFProjectile::OnBeginHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		if (ASFCharacter* SFCharacter = Cast<ASFCharacter>(OtherActor))
		{
			// 서버의 Projectile에서만 데미지 처리
			if (SFCharacter->HasAuthority())
			{
				SFCharacter->OnDamage(Damage, GetInstigator());
			}
		}

		OnHit.Broadcast();
		Destroy();
	}
}

