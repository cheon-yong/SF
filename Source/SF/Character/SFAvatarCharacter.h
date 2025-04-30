// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SFPlayerCharacter.h"
#include "SFAvatarCharacter.generated.h"

class USFAnimInstance;
class USFAnimInstance_Copy;

USTRUCT(BlueprintType)
struct FSFAnimData
{
	GENERATED_BODY()

	FSFAnimData() {}

	FSFAnimData(FVector InVelocity, float InGroundSpeed, bool InbShouldMove, bool InbIsFalling)
		: Velocity(InVelocity), GroundSpeed(InGroundSpeed), bShouldMove(InbShouldMove), bIsFalling(InbIsFalling)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling;
};

/**
 * 
 */
UCLASS()
class SF_API ASFAvatarCharacter : public ASFPlayerCharacter
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetTarget(ASFPlayerCharacter* InTargetCharacter);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetColor() override;
	
	void SetTargetAnimInstance();
	
	void CopyState();

	UFUNCTION()
	void OnRep_TargetCharacter();

	FVector GetPlayerThemeOffset() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_TargetCharacter, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ASFPlayerCharacter> TargetCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USFAnimInstance> TargetAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USFAnimInstance_Copy> AvartarAnim;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bFutureTheme = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;
};
