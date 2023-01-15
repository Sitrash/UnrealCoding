// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/BoxComponent.h"
#include "Inventory/Items/Item.h"
#include "PhysInteractActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UItem;

UCLASS()
class NICEIMMERSIVE_API APhysInteractActor : public AInteractActor
{
    GENERATED_BODY()

public:
    APhysInteractActor();

    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* StatMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* CollisionComp;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "InventoryItem")
    UItem* ItemData = nullptr;

protected:
    virtual void BeginPlay() override;
};
