// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/AIComponents/NiceAIPerceptionComponent.h"
#include "NiceAIController.generated.h"

class UNiceAIPerceptionComponent;
class URespawnComponent;

UCLASS()
class NICEIMMERSIVE_API ANiceAIController : public AAIController
{
    GENERATED_BODY()

public:
    ANiceAIController();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UNiceAIPerceptionComponent* NiceAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    URespawnComponent* RespawnComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
};
