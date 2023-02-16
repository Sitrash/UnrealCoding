// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersive/Inventory/InventoryUI/SubWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "NiceImmersive/Inventory/Items/Item.h"
#include "Components/Button.h"

void USubWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Use->OnClicked.AddDynamic(this, &ThisClass::OnUseItem);
    Drops->OnClicked.AddDynamic(this, &ThisClass::OnDropItem);
}

void USubWidget::OnUseItem()
{
    ItemRef->Use();
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
    RemoveFromParent();
}

void USubWidget::OnDropItem()
{
    ItemRef->Drop();
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
    RemoveFromParent();
}
