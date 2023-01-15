// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIShooterGame/SpectatorWidget.h"
#include "UtilsNiceImmersive.h"
#include "Components/RespawnComponent.h"

bool USpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComponent = AUtilsNiceImmersive::GetNicePlayerComponent<URespawnComponent>(GetOwningPlayer());

    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

    CountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}
