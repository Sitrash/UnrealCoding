// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "NeedAmmoDecorator.generated.h"

class ABaseWeapon;

UCLASS()
class NICEIMMERSIVE_API UNeedAmmoDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UNeedAmmoDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<ABaseWeapon> WeaponType;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
