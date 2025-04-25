// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HpBar.h"

#include "Components/ProgressBar.h"
#include "Character/SFCharacter.h"

UHpBar::UHpBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UHpBar::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(HpProgressBar);

	ASFCharacter* CharacterWidget = Cast<ASFCharacter>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UHpBar::UpdateHp(const uint8& InCurrentHp, const uint8& InMaxHp)
{
	CurrentHp = InCurrentHp;
	MaxHp = InMaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent((float)CurrentHp / MaxHp);
	}
}
