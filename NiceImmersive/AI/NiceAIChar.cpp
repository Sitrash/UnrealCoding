// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceAIChar.h"
#include "NiceAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AIComponents/AIWeaponComponent.h"
#include "BrainComponent.h"

ANiceAIChar::ANiceAIChar(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UAIWeaponComponent>("Weapon Component"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ANiceAIChar::OnDeath()
{
    Super::OnDeath();

    const auto NiceController = Cast<AAIController>(Controller);
    if (NiceController && NiceController->BrainComponent)
    {
        NiceController->BrainComponent->Cleanup();
    }
}
