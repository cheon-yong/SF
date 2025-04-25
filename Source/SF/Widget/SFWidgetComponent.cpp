// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SFWidgetComponent.h"
#include "Widget/SFUserWidget.h"

void USFWidgetComponent::InitWidget()
{
	Super::InitWidget();

	USFUserWidget* SFUserWidget = Cast<USFUserWidget>(GetWidget());
	if (SFUserWidget)
	{
		SFUserWidget->SetOwningActor(GetOwner());
	}
}
