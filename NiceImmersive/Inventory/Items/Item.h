// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Item.generated.h"

class UStaticMesh;
class UWeaponComponent;
class UHealthComponent;
class APhysInteractActor;
class UTexture2D;
class UInventoryComponent;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class NICEIMMERSIVE_API UItem : public UObject
{
    GENERATED_BODY()

public:
    virtual class UWorld* GetWorld() const { return World; }

    UPROPERTY(Transient)
    class UWorld* World;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FText UseActionText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    UStaticMesh* PickupMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    FText ItemDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
    FText ItemDescription;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    TSubclassOf<APhysInteractActor> SpawningActorItem;

    UPROPERTY()
    UInventoryComponent* OwningInventory;

    UFUNCTION(BlueprintCallable)
    virtual void Use();

    UFUNCTION(BlueprintImplementableEvent)
    void OnUse();

    UFUNCTION(BlueprintCallable)
    virtual void Drop();

    virtual void SpawnActorItem(UItem* ItemDates);

    virtual void AddingItem();

    UWeaponComponent* UItem::GetWeaponComponent() const;
    UHealthComponent* UItem::GetHealthComponent() const;
};
