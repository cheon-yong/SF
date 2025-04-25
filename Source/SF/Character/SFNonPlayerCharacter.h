// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/SFCharacter.h"
#include "SFNonPlayerCharacter.generated.h"

class UAnimMontage;
class ASFProjectile;
class UWidgetComponent;
class USFUserWidget;


DECLARE_DELEGATE(FAICharacterAttackFinished);

UCLASS()
class SF_API ASFNonPlayerCharacter : public ASFCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASFNonPlayerCharacter();

	float GetAIPatrolRadius();
	float GetAITurnSpeed();
	float GetAIAttackRange();
	float GetAIDetectRadius();

	// TODO : 수정해야함 빠르게 하기에는 좋지만 확장성이 너무 떨어짐
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished);
	virtual void AttackByAI();
	virtual void CreateProjectile();

	virtual void OnDamage(uint8 Damage, AActor* InInstigator) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	FAICharacterAttackFinished OnAttackFinished;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebug = false;

	UPROPERTY(VisibleAnywhere	, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<APawn> Target;

protected: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USFUserWidget> HpBarClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<ASFProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FName SpawnSocketName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float TurnSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectRadius = 200.f;

};
