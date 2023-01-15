// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/FindHearingSound.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "UtilsNiceImmersive.h"
#include "NiceFindEnemyService.h"
#include "Perception/AISense_Hearing.h"
#include "Components/AIComponents/NiceAIPerceptionComponent.h"

UFindHearingSound::UFindHearingSound()
{
    NodeName = "Find Hearing Sound";
}

void UFindHearingSound::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UNiceAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            if (PerceptionComponent->GetClosestVector() == FVector(0, 0, 0))
            {
                return;
            }
            else
            {
                Blackboard->SetValueAsVector(HearingKey.SelectedKeyName, PerceptionComponent->GetClosestVector());
            }
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
