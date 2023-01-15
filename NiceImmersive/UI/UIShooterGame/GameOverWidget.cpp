// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIShooterGame/GameOverWidget.h"
#include "GameMode/NiceImmersiveGameMode.h"
#include "GameMode/NicePlayerState.h"
#include "PlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "UtilsNiceImmersive.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"

void UGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ANiceImmersiveGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &UGameOverWidget::OnMatchStateChanged);
        }
    }

    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnResetLevel);
    }
}

void UGameOverWidget::OnMatchStateChanged(EMatchState State)
{
    if (State == EMatchState::GameOver)
    {
        UpdatePlayersStat();
    }
}

void UGameOverWidget::UpdatePlayersStat()
{
    if (!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ANicePlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<UPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(AUtilsNiceImmersive::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(AUtilsNiceImmersive::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(AUtilsNiceImmersive::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void UGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
