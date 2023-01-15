// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractActor.h"
#include "GrabInteractActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class NICEIMMERSIVE_API AGrabInteractActor : public AInteractActor
{
    GENERATED_BODY()
public:
    AGrabInteractActor();

protected:
    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* StatMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* CollisionComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "For grab objects")
    UMaterialInterface* OpacityMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "For grab objects")
    UMaterialInterface* SolidMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "For grab objects")
    float Impulse = 1500.0f;

    virtual void BeginPlay() override;
};
