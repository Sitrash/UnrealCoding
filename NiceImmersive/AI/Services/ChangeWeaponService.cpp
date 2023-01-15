// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeWeaponService.h"
#include "Components/WeaponComponent.h"
#include "AIController.h"
#include "UtilsNiceImmersive.h"

UChangeWeaponService::UChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void UChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    const auto Controller = OwnerComp.GetAIOwner();
    if (Controller)
    {
        const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}