// Fill out your copyright notice in the Description page of Project Settings.

#include "NicePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

void ANicePlayerState::LogInfo()
{
    UE_LOG(LogPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
