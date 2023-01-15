// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NiceImmersiveCharacter.h"
#include "NiceAIChar.generated.h"

class UBehaviorTree;

UCLASS()
class NICEIMMERSIVE_API ANiceAIChar : public ANiceImmersiveCharacter
{
    GENERATED_BODY()

public:
    ANiceAIChar(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

protected:
    virtual void OnDeath() override;
};
