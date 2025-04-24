// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Character/SFNonPlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ASFNonPlayerCharacter* NPC = Cast<ASFNonPlayerCharacter>(ControllingPawn);
	if (nullptr == NPC)
	{
		return EBTNodeResult::Failed;
	}

	//FAICharacterAttackFinished OnAttackFinished;
	//OnAttackFinished.BindLambda(
	//	[&]()
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	}
	//);

	//AIPawn->SetAIAttackDelegate(OnAttackFinished);
	//AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}