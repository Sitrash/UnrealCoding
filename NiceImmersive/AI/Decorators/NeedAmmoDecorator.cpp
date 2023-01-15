// Fill out your copyright notice in the Description page of Project Settings.

#include "NeedAmmoDecorator.h"
#include "AI/NiceAIController.h"
#include "UtilsNiceImmersive.h"
#include "Components/WeaponComponent.h"

UNeedAmmoDecorator::UNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool UNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->NeedAmmo(WeaponType);
}