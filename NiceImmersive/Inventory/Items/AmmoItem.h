// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Items/Item.h"
#include "AmmoItem.generated.h"

class ABaseWeapon;

UCLASS()
class NICEIMMERSIVE_API UAmmoItem : public UItem
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    int32 ClipsAmount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    TSubclassOf<ABaseWeapon> WeaponType;

protected:
    virtual void Drop() override;
    virtual void AddingItem() override;
};
