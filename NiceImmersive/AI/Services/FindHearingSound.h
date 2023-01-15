// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindHearingSound.generated.h"

UCLASS()
class NICEIMMERSIVE_API UFindHearingSound : public UBTService
{
    GENERATED_BODY()

public:
    UFindHearingSound();
    FTimerHandle HearingTimer;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector HearingKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
