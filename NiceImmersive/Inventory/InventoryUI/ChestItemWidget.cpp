// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersive/Inventory/InventoryUI/ChestItemWidget.h"
#include "Components/Button.h"
#include "NiceImmersive/Inventory/Items/Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NiceImmersive/Interactable/ChestInteract.h"
#include "ChestWidget.h"

void UChestItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UseButton->OnClicked.AddDynamic(this, &ThisClass::OnAddItems);
}

void UChestItemWidget::SetAllNice(UItem* ItemRef)
{
    if (!Icon) return;
    Icon->SetBrushFromTexture(ItemRef->Icon);
    if (!ItemName) return;
    ItemName->SetText(ItemRef->ItemDisplayName);
}

void UChestItemWidget::OnAddItems()
{
    if (ParentWidget->LastNum)
    {
        Chest->AddRemoveItem(ItemReference);
        ParentWidget->CloseInventory();
    }
    else
    {
        Chest->AddRemoveItem(ItemReference);
    }
}

void UChestItemWidget::NativeDestruct()
{
    Chest = nullptr;
    Super::NativeDestruct();
}
