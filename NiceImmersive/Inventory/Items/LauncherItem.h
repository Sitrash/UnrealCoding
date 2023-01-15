// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Items/Item.h"
#include "LauncherItem.generated.h"

class UActorComponent;

UCLASS()
class NICEIMMERSIVE_API ULauncherItem : public UItem
{
    GENERATED_BODY()

protected:
    virtual void Drop() override;
};
