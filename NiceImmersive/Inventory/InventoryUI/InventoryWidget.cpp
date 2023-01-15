// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryUI/InventoryWidget.h"
#include "Inventory/Items/Item.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryUI/InventoryItemWidget.h"
#include "Components/WrapBox.h"

void UInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InventoryReference = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
    UpdateInventory();
    InventoryReference->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);
}

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    GetOwningPlayer()->bShowMouseCursor = true;
    GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
}

void UInventoryWidget::UpdateInventory()
{
    InventoryBox->ClearChildren();
    for (auto const& InventoryItem : InventoryReference->Items)
    {
        if (!InventoryItem) continue;
        const auto ItemSlot = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), InventoryItemClass);
        ItemSlot->SetAllNice(InventoryItem);
        ItemSlot->ItemReference = InventoryItem;
        InventoryBox->AddChildToWrapBox(ItemSlot);
    }
}

void UInventoryWidget::CloseInventory()
{
    GetOwningPlayer()->bShowMouseCursor = false;
    GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
    RemoveFromParent();
}
