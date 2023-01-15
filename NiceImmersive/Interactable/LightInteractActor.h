// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "Components/PointLightComponent.h"
#include "LightInteractActor.generated.h"

class UBoxComponent;

UCLASS()
class NICEIMMERSIVE_API ALightInteractActor : public AInteractActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    ALightInteractActor();

    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPointLightComponent* Light;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* StatMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* CollisionComp;

    bool bIsOn = false;
};
