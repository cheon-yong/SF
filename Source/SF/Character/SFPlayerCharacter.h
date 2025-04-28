// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SFCharacter.h"
#include "SFPlayerCharacter.generated.h"

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

protected:

	// To add mapping context
	virtual void BeginPlay() override;

	void SetColor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Pitch();

public:
	UPROPERTY(ReplicatedUsing = OnRep_Pitch, VisibleAnywhere, BlueprintReadOnly)
	float Pitch_SideScroll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToMouseVector;
	


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color, meta = (AllowPrivateAccess = "true"))
	TArray<FCharacterMaterials> CharacterColorMaterials;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

};
