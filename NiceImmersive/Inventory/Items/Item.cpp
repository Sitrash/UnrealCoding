// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Interactable/PhysInteractActor.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "Inventory/InventoryComponent.h"

void UItem::Use() {}

void UItem::Drop() {}

void UItem::AddingItem() {}

void UItem::SpawnActorItem(UItem* ItemDates)
{
    const auto Pawn = Cast<ACharacter>(OwningInventory->GetOwner());
    if (!Pawn) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Pawn;
    FVector Loc = Pawn->GetCapsuleComponent()->GetComponentLocation() + Pawn->GetCapsuleComponent()->GetForwardVector() * 200;
    FRotator Rot = Rot.ZeroRotator;
    const auto ActorToSpawn =
        GetWorld()->SpawnActor<APhysInteractActor>(SpawningActorItem, Loc, Rot, SpawnParams);
    if (!ActorToSpawn) return;

    ActorToSpawn->ItemData = ItemDates;
    ActorToSpawn->StatMesh->SetStaticMesh(ActorToSpawn->ItemData->PickupMesh);
}

UWeaponComponent* UItem::GetWeaponComponent() const
{
    return OwningInventory->GetOwner()->FindComponentByClass<UWeaponComponent>();
}

UHealthComponent* UItem::GetHealthComponent() const
{
    return OwningInventory->GetOwner()->FindComponentByClass<UHealthComponent>();
}
