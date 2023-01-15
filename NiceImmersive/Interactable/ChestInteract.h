// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractActor.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/BoxComponent.h"
#include "Inventory/Items/Item.h"
#include "ChestInteract.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UItem;
class ANiceImmersiveCharacter;

UCLASS()
class NICEIMMERSIVE_API AChestInteract : public AInteractActor
{
    GENERATED_BODY()

public:
    AChestInteract();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* StatMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* CollisionComp;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "InventoryItem")
    UItem* ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventoryComponent* ChestInventoryComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ChestWidget")
    TSubclassOf<UUserWidget> ChestWidgetClass;

    UFUNCTION(BlueprintCallable)
    void AddRemoveItem(UItem* ItemRef);

    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn) override;

    bool WidgetIsOpened = false;
    ANiceImmersiveCharacter* CharacterToInteract = nullptr;

protected:
    virtual void BeginPlay() override;
};
