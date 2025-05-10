// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/SFCharacter.h"
#include "SFPlayerCharacter.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SF_API ASFPlayerCharacter : public ASFCharacter
{
	GENERATED_BODY()
	

public:
	ASFPlayerCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Reliable)
	void Server_UpdateAim(float NewPitch);

	UFUNCTION(BlueprintCallable)
	void ChangeCameraComponent();

	void Respawn();

	TArray<AActor*> GetInteractActors();

protected:

	// To add mapping context
	virtual void BeginPlay() override;

	virtual void SetColor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Pitch();

	virtual void OnSpawn() override;

	virtual void OnDeath() override;

	UFUNCTION()
	void OnInteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(ReplicatedUsing = OnRep_Pitch, VisibleAnywhere, BlueprintReadOnly)
	float Pitch_SideScroll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToMouseVector;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> InteractBox;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color, meta = (AllowPrivateAccess = "true"))
	TArray<FCharacterMaterials> CharacterColorMaterials;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraComponent> SplitCameraComponentClass;
};
