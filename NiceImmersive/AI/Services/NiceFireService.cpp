// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "UtilsNiceImmersive.h"
#include "Components/WeaponComponent.h"

UNiceFireService::UNiceFireService()
{
    NodeName = "Fire";
}

void UNiceFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent)
        {
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
