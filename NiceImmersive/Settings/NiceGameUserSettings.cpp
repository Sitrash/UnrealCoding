// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/NiceGameUserSettings.h"
#include "Settings/NiceSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 Level)             \
    {                            \
        FUNC(Level);             \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UNiceGameUserSettings::UNiceGameUserSettings()
{
    const TArray<FSettingOption> VFXOptions = {{LOCTEXT("VFXQualityLow_Loc", "Low"), 0}, {LOCTEXT("VFXQualityMedium_Loc", "Medium"), 1},
        {LOCTEXT("VFXQualityHigh_Loc", "High"), 2}, {LOCTEXT("VFXQualityEpic_Loc", "Epic"), 3}};

    {
        auto* Setting = NewObject<UNiceSetting>();
        Setting->SetName(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UNiceSetting>();
        Setting->SetName(LOCTEXT("Textures_Loc", "Textures"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UNiceSetting>();
        Setting->SetName(LOCTEXT("Shadows_Loc", "Shadows"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetShadowQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadowQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UNiceSetting>();
        Setting->SetName(LOCTEXT("PostProcess_Loc", "Post Processing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UNiceSetting>();
        Setting->SetName(LOCTEXT("VisualEffects_Loc", "Effects"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetVisualEffectQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetVisualEffectQuality));
        VideoSettings.Add(Setting);
    }
}

UNiceGameUserSettings* UNiceGameUserSettings::Get()
{
    return GEngine ? Cast<UNiceGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UNiceSetting*>& UNiceGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void UNiceGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE