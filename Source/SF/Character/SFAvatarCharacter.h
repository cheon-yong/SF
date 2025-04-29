// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SFPlayerCharacter.h"
#include "SFAvatarCharacter.generated.h"

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

	void CopyState();

	FVector GetPlayerThemeOffset() const;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ASFPlayerCharacter> TargetCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFutureTheme = true;
};
