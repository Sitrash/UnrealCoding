// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysInteractActor.h"
#include "Components/BoxComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Character/NiceImmersiveCharacter.h"

APhysInteractActor::APhysInteractActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StatMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    StatMesh->SetupAttachment(RootComponent);

    CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionComp");
    CollisionComp->SetupAttachment(StatMesh);

    InteractionWidget->SetupAttachment(CollisionComp);
}

void APhysInteractActor::BeginPlay()
{
    Super::BeginPlay();

    if (ItemData)
    {
        StatMesh->SetStaticMesh(ItemData->PickupMesh);
    }
}

void APhysInteractActor::InteractWithMe(ANiceImmersiveCharacter* Pawn)
{
    if (Pawn)  //(const auto Pawn = Cast<ANiceImmersiveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if (const auto InvComp = Pawn->FindComponentByClass<UInventoryComponent>())
        {
            if (InvComp->AddItem(ItemData))
            {
                Destroy();
            }
        }
    }
}
