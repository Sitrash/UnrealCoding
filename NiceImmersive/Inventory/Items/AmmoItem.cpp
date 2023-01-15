// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Items/AmmoItem.h"
#include "Inventory/InventoryComponent.h"
#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"

void UAmmoItem::Drop()
{
    SpawnActorItem(this);
    OwningInventory ? OwningInventory->RemoveItem(this) : nullptr;
}

void UAmmoItem::AddingItem()
{
    if (!GetWeaponComponent()) return;
    GetWeaponComponent()->TryToAddAmmo(WeaponType, ClipsAmount);
}
