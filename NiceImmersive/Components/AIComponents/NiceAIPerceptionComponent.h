// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "NiceAIPerceptionComponent.generated.h"

UCLASS()
class NICEIMMERSIVE_API UNiceAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy() const;
    FVector GetClosestVector();
};
