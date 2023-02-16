// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "NiceImmersive/CoreTypes.h"
#include "WeaponItem.generated.h"

class ABaseWeapon;

UCLASS()
class NICEIMMERSIVE_API UWeaponItem : public UItem
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 IndexWeapon;

protected:
    virtual void Use() override;
    virtual void Drop() override;
    virtual void AddingItem() override;
};
