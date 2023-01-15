// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractActor.h"

AInteractActor::AInteractActor()
{
    PrimaryActorTick.bCanEverTick = false;

    _RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
    RootComponent = _RootComponent;

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>("Interaction Widget");
    InteractionWidget->SetupAttachment(RootComponent);
}

void AInteractActor::BeginPlay()
{
    Super::BeginPlay();

    InteractionWidget->SetVisibility(false);
}

void AInteractActor::InteractWithMe(ANiceImmersiveCharacter* Pawn) {}

void AInteractActor::ShowInteractionWidget()
{
    InteractionWidget->SetVisibility(true);
}

void AInteractActor::HideInteractionWidget()
{
    InteractionWidget->SetVisibility(false);
}
