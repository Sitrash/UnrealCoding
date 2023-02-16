// Fill out your copyright notice in the Description page of Project Settings.


#include "NiceImmersive/UI/BaseWidget.h"
#include "NiceImmersive/Character/NiceImmersiveCharacter.h"

void UBaseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    PlayerCharacter = Cast<ANiceImmersiveCharacter>(GetOwningPlayerPawn());
    PawnController = GetOwningPlayer();
}