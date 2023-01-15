// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Items/FoodItem.h"
#include "Inventory/InventoryComponent.h"
#include "Components/HealthComponent.h"

void UFoodItem::Use()
{
    if (!GetHealthComponent()) return;

    GetHealthComponent()->TryToAddHealth(HealthToHeal);
    OwningInventory->RemoveItem(this);
}

void UFoodItem::Drop()
{
    SpawnActorItem(this);
    OwningInventory ? OwningInventory->RemoveItem(this) : nullptr;
}
