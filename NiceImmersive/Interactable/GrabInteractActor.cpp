// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/GrabInteractActor.h"
#include "Components/BoxComponent.h"
#include "Character/NiceImmersiveCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AGrabInteractActor::AGrabInteractActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StatMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    StatMesh->SetupAttachment(RootComponent);

    CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionComp");
    CollisionComp->SetupAttachment(StatMesh);

    InteractionWidget->SetupAttachment(CollisionComp);
}

void AGrabInteractActor::BeginPlay()
{
    Super::BeginPlay();
}

void AGrabInteractActor::InteractWithMe(ANiceImmersiveCharacter* Pawn)
{
    const auto WeaponComp = Pawn->FindComponentByClass<UWeaponComponent>();
    if (WeaponComp->EquipAnimInProgress || WeaponComp->ReloadAnimInProgress) return;

    if (!Pawn->GetHolding())
    {
        AttachToComponent(Pawn->GetDragHand(),
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true));
        StatMesh->SetSimulatePhysics(false);
        StatMesh->AttachToComponent(Pawn->GetDragHand(),
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true));
        StatMesh->SetMaterial(0, OpacityMaterial);
        Pawn->SubFuncForHolding(this, true);

        InteractionWidget->SetHiddenInGame(true);
    }
    else
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        StatMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        StatMesh->SetSimulatePhysics(true);
        StatMesh->SetMaterial(0, SolidMaterial);

        if (Pawn->CanThrowing())
        {
            FVector Forward = WeaponComp->CharacterGrabActor->GetActorForwardVector();
            StatMesh->AddImpulse(Forward * Impulse * StatMesh->GetMass());
        }

        Pawn->SetThrowing(false);
        Pawn->SubFuncForHolding(nullptr, false);

        InteractionWidget->SetHiddenInGame(false);
    }
}
