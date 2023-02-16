// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/ChestInteract.h"
#include "Character/NiceImmersiveCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryUI/ChestWidget.h"
#include "Components/ActionComponent.h"

AChestInteract::AChestInteract()
{
    PrimaryActorTick.bCanEverTick = false;

    StatMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    StatMesh->SetupAttachment(RootComponent);
    CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionComp");
    CollisionComp->SetupAttachment(StatMesh);
    InteractionWidget->SetupAttachment(StatMesh);
    ChestInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("ChestInventory");
}

void AChestInteract::BeginPlay()
{
    Super::BeginPlay();
}

void AChestInteract::InteractWithMe(ANiceImmersiveCharacter* Pawn)
{
    const auto ActionComponent = Pawn->ActionComponent;
    CharacterToInteract = Pawn;

    if (ActionComponent->IsShowingInventory) return;
    if (WidgetIsOpened) return;

    const auto ChestWidget = CreateWidget<UChestWidget>(GetWorld(), ActionComponent->ChestWidgetClass);
    if (!ChestWidget) return;
    ChestWidget->ChestActor = this;
    ChestWidget->AddToViewport();

    WidgetIsOpened = true;
    ActionComponent->CanOpenInventory = false;
}

void AChestInteract::AddRemoveItem(UItem* ItemRef)
{
    const auto InventoryComp = CharacterToInteract->InventoryComponent;
    if (!InventoryComp) return;
    // if (InventoryComp->AddItem(ItemRef)) ChestInventoryComponent->RemoveChestItem(ItemRef);
    if (ChestInventoryComponent->RemoveItem(ItemRef)) InventoryComp->AddItem(ItemRef);
}