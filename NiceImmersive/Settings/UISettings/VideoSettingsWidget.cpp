// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/UISettings/VideoSettingsWidget.h"
#include "Settings/NiceGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "SettingOptionWidget.h"
#include "Components/Button.h"

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UNiceGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogTemp, Warning, TEXT("SettingsNotExist"));
        return;
    }

    UserSettings->LoadSettings();
    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting : VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton) RunBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnBenchmark);

    UserSettings->OnVideoSettingsUpdated.AddUObject(this, &ThisClass::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto* UserSettings = UNiceGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!VideoSettingsContainer) return;
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingOptionWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingOptionWidget->UpdateTexts();
        }
    }
}
