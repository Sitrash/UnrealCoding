// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersive/Components/ActionComponent.h"
#include "NiceImmersive/Character/NiceImmersiveCharacter.h"
#include "NiceImmersive/UI/BaseWidget.h"
#include "NiceImmersive/Inventory/InventoryUI/InventoryWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiceImmersive/Components/WeaponComponent.h"

UActionComponent::UActionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UActionComponent::BeginPlay()
{
    Super::BeginPlay();

    Pawn = Cast<ANiceImmersiveCharacter>(GetOwner());
    MyBaseWidget = CreateWidget<UBaseWidget>(GetWorld(), BaseWidgetClass);
    MyBaseWidget->AddToViewport();
    MyBaseWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UActionComponent::OnInventoryShow()
{
    if (ForbidShowInventory()) return;

    if (!IsShowingInventory)
    {
        if (InventoryWidget)
        {
            Widget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidget);
            if (!Widget) return;
            Widget->AddToViewport();
            Pawn->GetController<APlayerController>()->SetMouseLocation(MyBaseWidget->XCoord, MyBaseWidget->YCoord);
        }
    }
    else
    {
        Pawn->GetController<APlayerController>()->GetMousePosition(MyBaseWidget->XCoord, MyBaseWidget->YCoord);
        Widget->CloseInventory();
    }
}

bool UActionComponent::ForbidShowInventory() 
{
    if (Pawn->GetController<APlayerController>()->IsPaused() || !CanOpenInventory)
        return true;
    else
        return false;
}

void UActionComponent::OnAction()
{
    if (Pawn->Interface) Pawn->Interface->InteractWithMe(Pawn);
}

void UActionComponent::OnFire()
{
    if (bHolding)
    {
        CanThrow = true;
        Pawn->Interface->InteractWithMe(Pawn);
    }
}

// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

void UActionComponent::SubFuncForHolding(AActor* HoldingActor, bool IsHolding)
{
    const auto WeaponComp = GetOwner()->FindComponentByClass<UWeaponComponent>();
    bHolding = IsHolding;
    if (!bHolding)
    {
        WeaponComp->CharacterGrabActor = nullptr;
        WeaponComp->IsEquip() ? WeaponComp->EquipWeapon(WeaponComp->GetCurrentWeaponIndex()) : nullptr;
        Pawn->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    }
    else
    {
        WeaponComp->CharacterGrabActor = HoldingActor;
        WeaponComp->UnEquipForDrop();
        Pawn->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    }
}
