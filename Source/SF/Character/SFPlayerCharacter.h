// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/SFCharacter.h"
#include "SFPlayerCharacter.generated.h"

class UBoxComponent;
class UPhysicsHandleComponent;

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

	void Interact();


	UFUNCTION(BlueprintCallable)
	void PlayAnimMontageAndBlockMove(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FVector StartingPosition = FVector(0.f, 0.f, 0.f), FString StartingSection = TEXT("None"));

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayAnimMontageAndBlockMove(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FVector StartingPosition = FVector(0.f, 0.f, 0.f), const FString& StartingSection = TEXT("None"));

	UFUNCTION(Server, Unreliable)
	void Server_PlayAnimMontageAndBlockMove(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FVector StartingPosition = FVector(0.f, 0.f, 0.f), const FString& StartingSection = TEXT("None"));

	TArray<AActor*> GetInteractActors();

	void WallJump(UAnimMontage* WallJumpMontage);

	void ResetOption();

	void SetSideOption(FRotator TargetRotationRate);

	void PlayMontage(UAnimMontage* Montage);

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

	void PlayAnimMontageAndBlockMove_Internal(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FVector StartingPosition = FVector(0.f, 0.f, 0.f), FString StartingSection = TEXT("None"));

	void Interact_Internal();

	UFUNCTION(Server, Unreliable)
	void Server_Interact();

	void WallJump_Internal(UAnimMontage* WallJumpMontage);

	UFUNCTION(Server, Unreliable)
	void Server_WallJump(UAnimMontage* WallJumpMontage);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ResetOption();

	UFUNCTION(Server, Reliable)
	void Server_ResetOption();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetSideOption(FRotator TargetRotationRate);

	UFUNCTION(Server, Reliable)
	void Server_SetSideOption(FRotator TargetRotationRate);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayMontage(UAnimMontage* Montage);

	UFUNCTION(Server, Unreliable)
	void Server_PlayAnimMontage(UAnimMontage* Montage);

public:
	UPROPERTY(ReplicatedUsing = OnRep_Pitch, VisibleAnywhere, BlueprintReadOnly)
	float Pitch_SideScroll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToMouseVector;


	// Debug
	// MyPlayerController.h 또는 원하는 클래스에 추가
	FTimerHandle CameraDebugTimerHandle;

	UFUNCTION()
	void LogCameraState();
	// ~ Debug


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator OriginRate;
};
