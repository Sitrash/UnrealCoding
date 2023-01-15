// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/WidgetComponent.h"
#include "InteractActor.generated.h"

UCLASS()
class NICEIMMERSIVE_API AInteractActor : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AInteractActor();

protected:
    virtual void BeginPlay() override;

    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn) override;
    virtual void ShowInteractionWidget() override;
    virtual void HideInteractionWidget() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* _RootComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UWidgetComponent* InteractionWidget;
};
