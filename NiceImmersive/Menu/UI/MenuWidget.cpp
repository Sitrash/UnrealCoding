// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/MenuWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/NiceGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelItemWidget.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuWidget, All, All);

void UMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void UMenuWidget::InitLevelItems()
{
    const auto NiceGameInstance = GetNiceGameInstance();
    if (!NiceGameInstance) return;

    checkf(NiceGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    for (auto LevelData : NiceGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<ULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelitemWidgets.Add(LevelItemWidget);
    }

    if (NiceGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(NiceGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(NiceGameInstance->GetStartupLevel());
    }
}

void UMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto NiceGameInstance = GetNiceGameInstance();
    if (!NiceGameInstance) return;

    NiceGameInstance->SetStartupLevel(Data);

    for (auto LevelItemWidget : LevelitemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

void UMenuWidget::OnStartGame()
{
    PlayAnimation(HideAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation != HideAnimation) return;

    const auto NiceGameInstance = GetNiceGameInstance();
    if (!NiceGameInstance) return;

    UGameplayStatics::OpenLevel(this, NiceGameInstance->GetStartupLevel().LevelName);
}

void UMenuWidget::OnQuitGame()
{

    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UNiceGameInstance* UMenuWidget::GetNiceGameInstance() const
{
    if (!GetWorld()) return nullptr;

    return GetWorld()->GetGameInstance<UNiceGameInstance>();
}
