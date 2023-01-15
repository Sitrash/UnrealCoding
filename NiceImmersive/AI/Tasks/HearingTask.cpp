// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/HearingTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiceImmersive/UtilsNiceImmersive.h"
#include "AIController.h"
#include "Components/AIComponents/NiceAIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Services/FindHearingSound.h"

UHearingTask::UHearingTask()
{
    bCreateNodeInstance = true;
}

EBTNodeResult::Type UHearingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto PerceptionComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UNiceAIPerceptionComponent>(Controller);
    if (!PerceptionComponent) return EBTNodeResult::Failed;

    PerceptionComponent->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
    Blackboard->ClearValue(HearingKey.SelectedKeyName);
    return EBTNodeResult::Succeeded;
}
