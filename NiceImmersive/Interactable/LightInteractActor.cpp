// Fill out your copyright notice in the Description page of Project Settings.

#include "LightInteractActor.h"
#include "Components/BoxComponent.h"

ALightInteractActor::ALightInteractActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StatMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    StatMesh->SetupAttachment(RootComponent);

    CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
    CollisionComp->SetupAttachment(StatMesh);

    Light = CreateDefaultSubobject<UPointLightComponent>("Light");
    Light->SetupAttachment(StatMesh);

    InteractionWidget->SetupAttachment(StatMesh);
}

void ALightInteractActor::BeginPlay()
{
    Super::BeginPlay();

    Light->SetIntensity(0);
}

void ALightInteractActor::InteractWithMe(ANiceImmersiveCharacter* Pawn)
{
    if (bIsOn)
    {
        Light->SetIntensity(0);
        bIsOn = false;
    }
    else
    {
        Light->SetIntensity(5000);
        bIsOn = true;
    }
}
