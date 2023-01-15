// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Items/WeaponItem.h"
#include "Inventory/InventoryComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"

void UWeaponItem::Use()
{
    if (!GetWeaponComponent()) return;

    if (GetWeaponComponent()->GetCurrentWeaponIndex() != IndexWeapon)
    {
        GetWeaponComponent()->EquipWeapon(IndexWeapon);
        GetWeaponComponent()->SetCurrentWeaponIndex(IndexWeapon);
    }
}

void UWeaponItem::Drop()
{
    if (!GetWeaponComponent()) return;

    if (GetWeaponComponent()->GetCurrentWeaponIndex() == IndexWeapon)
    {
        GetWeaponComponent()->UnEquip();
    }

    SpawnActorItem(this);
    OwningInventory ? OwningInventory->RemoveItem(this) : nullptr;
}

void UWeaponItem::AddingItem()
{
    if (!GetWeaponComponent()) return;

    if (!GetWeaponComponent()->IsHoldingWeapon())
    {
        GetWeaponComponent()->EquipWeapon(IndexWeapon);
        GetWeaponComponent()->SetCurrentWeaponIndex(IndexWeapon);
    }
}
