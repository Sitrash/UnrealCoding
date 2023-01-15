// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceAIController.h"
#include "NiceAIChar.h"
#include "Components/AIComponents/NiceAIPerceptionComponent.h"
#include "Components/RespawnComponent.h"
#include "Character/NiceImmersiveCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ANiceAIController::ANiceAIController()
{
    NiceAIPerceptionComponent = CreateDefaultSubobject<UNiceAIPerceptionComponent>("NiceAIPerceptionComponent");
    SetPerceptionComponent(*NiceAIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
}

void ANiceAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ANiceAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto NiceCharacter = Cast<ANiceAIChar>(InPawn);
    if (NiceCharacter)
    {
        RunBehaviorTree(NiceCharacter->BehaviorTreeAsset);
    }
}

void ANiceAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ANiceAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
