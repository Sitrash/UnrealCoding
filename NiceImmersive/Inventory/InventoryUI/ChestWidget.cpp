// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryUI/ChestWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Interactable/ChestInteract.h"
#include "Components/WrapBox.h"
#include "Inventory/Items/Item.h"
#include "ChestItemWidget.h"
#include "Components/Button.h"
#include "Character/NiceImmersiveCharacter.h"

void UChestWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    GetOwningPlayer()->bShowMouseCursor = true;
    GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
    GetOwningPlayer()->SetMouseLocation(XCoord, YCoord);
}

void UChestWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ChestActor) InventoryReference = ChestActor->ChestInventoryComponent;

    UpdateInventory();

    InventoryReference->OnInventoryUpdated.AddDynamic(this, &ThisClass::UpdateInventory);
    CloseButton->OnClicked.AddDynamic(this, &ThisClass::CloseInventory);

    if (InventoryBox->GetAllChildren().Num() == 0)
        GetOwningPlayer()->SetMouseLocation(800, 450);  // TODO: THIS SHIT IS MAGIC NUMBERS. NEED REDONE
}

void UChestWidget::UpdateInventory()
{
    InventoryBox->ClearChildren();
    for (auto const& InventoryItem : InventoryReference->Items)
    {
        if (!InventoryItem) continue;
        const auto ItemSlot = CreateWidget<UChestItemWidget>(GetOwningPlayer(), ItemWidgetClass);
        ItemSlot->Chest = ChestActor;
        ItemSlot->ItemReference = InventoryItem;
        ItemSlot->ParentWidget = this;
        ItemSlot->SetAllNice(InventoryItem);
        InventoryBox->AddChildToWrapBox(ItemSlot);

        InventoryBox->GetAllChildren().Num() == 1 ? LastNum = true : LastNum = false;
    }
}

void UChestWidget::CloseInventory()
{
    GetOwningPlayer()->bShowMouseCursor = false;
    GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
    ChestActor->WidgetIsOpened = false;
    ChestActor->CharacterToInteract->CanOpenInventory = true;
    ChestActor->CharacterToInteract = nullptr;
    ChestActor = nullptr;
    RemoveFromParent();
}
