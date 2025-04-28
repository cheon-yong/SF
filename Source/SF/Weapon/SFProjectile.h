// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SFProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);


UCLASS()
class SF_API ASFProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ASFProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnBeginHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Damage = 0;

	UPROPERTY(BlueprintAssignable)
	FOnHit OnHit;

protected:
	
	/** Sphere collision component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
