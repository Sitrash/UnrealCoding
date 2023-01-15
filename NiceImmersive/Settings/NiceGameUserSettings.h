// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "NiceGameUserSettings.generated.h"

class UNiceSetting;

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

UCLASS()
class NICEIMMERSIVE_API UNiceGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UNiceGameUserSettings();
    static UNiceGameUserSettings* Get();

    const TArray<UNiceSetting*>& GetVideoSettings() const;

    void RunBenchmark();

    FOnSettingsUpdatedDelegate OnVideoSettingsUpdated;

private:
    UPROPERTY()
    TArray<UNiceSetting*> VideoSettings;
};
