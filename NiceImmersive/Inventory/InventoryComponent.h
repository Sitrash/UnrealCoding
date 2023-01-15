// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItem;
class APhysInteractActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NICEIMMERSIVE_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    bool AddItem(UItem* Item);

    UFUNCTION(BlueprintCallable)
    bool RemoveItem(UItem* Item);

    UPROPERTY(EditAnywhere, Instanced)
    TArray<UItem*> DefaultItems;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
    TArray<UItem*> Items;

    UFUNCTION(BlueprintCallable)
    bool RemoveChestItem(UItem* Item);

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int32 InventorySize = 16;

    TSubclassOf<APhysInteractActor> GetPhysActor() { return SpawnActor; }

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnItemActor")
    TSubclassOf<APhysInteractActor> SpawnActor;
};
