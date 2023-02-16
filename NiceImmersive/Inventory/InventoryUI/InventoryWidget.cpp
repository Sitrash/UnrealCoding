// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersive/Inventory/InventoryUI/InventoryWidget.h"
#include "NiceImmersive/Inventory/Items/Item.h"
#include "NiceImmersive/Inventory/InventoryComponent.h"
#include "NiceImmersive/Inventory/InventoryUI/InventoryItemWidget.h"
#include "Components/WrapBox.h"
#include "NiceImmersive/Character/NiceImmersiveCharacter.h"
#include "NiceImmersive/Components/ActionComponent.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InventoryReference = PlayerCharacter->InventoryComponent;
    UpdateInventory();
    InventoryReference->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);
    InputAction.BindDynamic(this, &UInventoryWidget::CloseInventory);
}

void UInventoryWidget::NativeConstruct() 
{
    Super::NativeConstruct();

    PlayerCharacter->ActionComponent->IsShowingInventory = true;
    PawnController->SetInputMode(FInputModeGameAndUI());
    PawnController->bShowMouseCursor = true;
    UGameplayStatics::PlaySound2D(GetWorld(), OpenInventorySound);
    ListenForInputAction(PauseGame, EInputEvent::IE_Pressed, true, InputAction);
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
    PawnController->SetInputMode(FInputModeGameOnly());
    PawnController->bShowMouseCursor = false;
    PlayerCharacter->ActionComponent->IsShowingInventory = false;
    UGameplayStatics::PlaySound2D(GetWorld(), CloseInventorySound);
    RemoveFromParent();
}
