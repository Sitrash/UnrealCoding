// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDataWidget.h"
#include "GameMode/NiceImmersiveGameMode.h"
#include "GameMode/NicePlayerState.h"

int32 UGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetNicePlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 UGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetNiceGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UGameDataWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetNiceGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetNiceGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ANiceImmersiveGameMode* UGameDataWidget::GetNiceGameMode() const
{
    return GetWorld() ? Cast<ANiceImmersiveGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ANicePlayerState* UGameDataWidget::GetNicePlayerState() const
{
    return GetOwningPlayer() ? Cast<ANicePlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
