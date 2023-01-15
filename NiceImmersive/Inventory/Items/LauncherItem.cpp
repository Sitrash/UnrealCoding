// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Items/LauncherItem.h"
#include "Inventory/InventoryComponent.h"
#include "Components/WeaponComponent.h"

void ULauncherItem::Drop()
{
    OwningInventory ? OwningInventory->RemoveItem(this) : nullptr;
}
