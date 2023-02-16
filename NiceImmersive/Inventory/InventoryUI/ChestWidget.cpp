// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersive/Inventory/InventoryUI/ChestWidget.h"
#include "NiceImmersive/Inventory/InventoryComponent.h"
#include "NiceImmersive/Interactable/ChestInteract.h"
#include "Components/WrapBox.h"
#include "NiceImmersive/Inventory/Items/Item.h"
#include "ChestItemWidget.h"
#include "Components/Button.h"
#include "NiceImmersive/Character/NiceImmersiveCharacter.h"
#include "NiceImmersive/Components/ActionComponent.h"
#include "Kismet/GameplayStatics.h"

void UChestWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    PawnController->bShowMouseCursor = true;
    PawnController->SetInputMode(FInputModeGameAndUI());
    PawnController->SetMouseLocation(XCoord, YCoord);
    InputAction.BindDynamic(this, &UChestWidget::CloseInventory);
}

void UChestWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ChestActor) InventoryReference = ChestActor->ChestInventoryComponent;

    UpdateInventory();

    InventoryReference->OnInventoryUpdated.AddDynamic(this, &UChestWidget::UpdateInventory);
    CloseButton->OnClicked.AddDynamic(this, &UChestWidget::CloseInventory);

    if (InventoryBox->GetAllChildren().Num() == 0)
        PawnController->SetMouseLocation(800, 450);  // TODO: THIS SHIT IS MAGIC NUMBERS. NEED REDONE

    UGameplayStatics::PlaySound2D(GetWorld(), OpenInventorySound);
    ListenForInputAction(PauseGame, EInputEvent::IE_Pressed, true, InputAction);
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
    PawnController->bShowMouseCursor = false;
    PawnController->SetInputMode(FInputModeGameOnly());
    ChestActor->WidgetIsOpened = false;
    PlayerCharacter->ActionComponent->CanOpenInventory = true;
    //ChestActor->CharacterToInteract = nullptr;
    ChestActor = nullptr;
    UGameplayStatics::PlaySound2D(GetWorld(), CloseInventorySound);
    RemoveFromParent();
}
