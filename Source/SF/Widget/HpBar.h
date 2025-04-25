// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/SFUserWidget.h"
#include "HpBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SF_API UHpBar : public USFUserWidget
{
	GENERATED_BODY()
	
public:
	UHpBar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateHp(const uint8& InCurrentHp, const uint8& InMaxHp);

protected:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HpProgressBar;

	UPROPERTY()
	uint8 CurrentHp;

	UPROPERTY()
	uint8 MaxHp;
};
