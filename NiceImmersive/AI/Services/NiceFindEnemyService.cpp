// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "UtilsNiceImmersive.h"
#include "Perception/AISense_Hearing.h"
#include "Components/AIComponents/NiceAIPerceptionComponent.h"

UNiceFindEnemyService::UNiceFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UNiceFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UNiceAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
