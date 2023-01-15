// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPercentDecorator.h"
#include "AI/NiceAIController.h"
#include "UtilsNiceImmersive.h"
#include "Components/HealthComponent.h"

UHealthPercentDecorator::UHealthPercentDecorator()
{
    NodeName = "HealthPercent";
}

bool UHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent;
}
