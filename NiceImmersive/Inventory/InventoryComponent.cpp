// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Items/Item.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    for (auto& Item : DefaultItems)
    {
        AddItem(Item);
    }
}

bool UInventoryComponent::AddItem(UItem* Item)
{
    if (Items.Num() == InventorySize || !Item) return false;

    Item->OwningInventory = this;
    Item->World = GetWorld();
    Items.Add(Item);
    Item->AddingItem();
    OnInventoryUpdated.Broadcast();

    return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
    if (Item)
    {
        Item->OwningInventory = nullptr;
        Item->World = nullptr;
        Items.RemoveSingle(Item);
        OnInventoryUpdated.Broadcast();
        return true;
    }
    return false;
}

bool UInventoryComponent::RemoveChestItem(UItem* Item)
{
    if (Item)
    {
        Items.RemoveSingle(Item);
        OnInventoryUpdated.Broadcast();
        return true;
    }
    return false;
}
