// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Items/Item.h"
#include "Inventory/Items/AmmoItem.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UseButton->OnClicked.AddDynamic(this, &ThisClass::ShowSubWidget);
    Use->OnClicked.AddDynamic(this, &ThisClass::OnUseItem);
    Drops->OnClicked.AddDynamic(this, &ThisClass::OnDropItem);
}

void UInventoryItemWidget::SetAllNice(UItem* ItemRef)
{
    if (!Icon) return;
    Icon->SetBrushFromTexture(ItemRef->Icon);
    if (!ItemName) return;
    ItemName->SetText(ItemRef->ItemDisplayName);
}

void UInventoryItemWidget::ShowSubWidget()
{
    UsesButtonsCanvas->GetVisibility() == ESlateVisibility::Visible ? 
        UsesButtonsCanvas->SetVisibility(ESlateVisibility::Collapsed) :
        UsesButtonsCanvas->SetVisibility(ESlateVisibility::Visible);

    const auto AmmoItem = Cast<UAmmoItem>(ItemReference);
    if (AmmoItem) Use->SetVisibility(ESlateVisibility::Hidden);

    UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UInventoryItemWidget::OnUseItem()
{
    ItemReference->Use();
    UsesButtonsCanvas->SetVisibility(ESlateVisibility::Collapsed);
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UInventoryItemWidget::OnDropItem()
{
    ItemReference->Drop();
    UsesButtonsCanvas->SetVisibility(ESlateVisibility::Collapsed);
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UInventoryItemWidget::NativeDestruct()
{
    UsesButtonsCanvas->SetVisibility(ESlateVisibility::Collapsed);
    UWidgetBlueprintLibrary::SetFocusToGameViewport();

    Super::NativeDestruct();
}
