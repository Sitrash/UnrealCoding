// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceGameInstance.h"
#include "SoundFuncLib.h"

void UNiceGameInstance::ToggleVolume()
{
    USoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
