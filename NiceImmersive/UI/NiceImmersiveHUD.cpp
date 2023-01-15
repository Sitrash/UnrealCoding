// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiceImmersiveHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/NiceImmersiveGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

ANiceImmersiveHUD::ANiceImmersiveHUD() {}

void ANiceImmersiveHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(EMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ANiceImmersiveGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ANiceImmersiveHUD::OnMatchStateChanged);
        }
    }
}

void ANiceImmersiveHUD::DrawHUD()
{
    Super::DrawHUD();
}

void ANiceImmersiveHUD::DrawCrosshair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThikness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThikness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThikness);
}

void ANiceImmersiveHUD::OnMatchStateChanged(EMatchState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(LogGameHUD, Display, TEXT("Match state change: %s"), *UEnum::GetValueAsString(State));
}