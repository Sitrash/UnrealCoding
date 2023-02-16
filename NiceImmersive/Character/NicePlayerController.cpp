// Fill out your copyright notice in the Description page of Project Settings.

#include "NicePlayerController.h"
#include "NiceImmersive/Components/RespawnComponent.h"
#include "NiceImmersive/GameMode/NiceImmersiveGameMode.h"
#include "NiceImmersive/UI/NiceImmersiveHUD.h"
#include "NiceImmersive/GameMode/NiceGameInstance.h"
#include "Blueprint/UserWidget.h"

ANicePlayerController::ANicePlayerController()
{
    RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnComponent");
}

void ANicePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        if (const auto GameMode = Cast<ANiceImmersiveGameMode>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ANicePlayerController::OnMatchStateChanged);
        }
    }
}

void ANicePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ANicePlayerController::OnPauseGame).bExecuteWhenPaused = true;
    InputComponent->BindAction("Mute", IE_Pressed, this, &ANicePlayerController::OnMuteSound).bExecuteWhenPaused = true;
}

void ANicePlayerController::OnPauseGame()
{
    SetPause(!IsPaused());
    bShowMouseCursor = IsPaused();
    IsPaused() ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false)) : SetInputMode(FInputModeGameOnly());
}

void ANicePlayerController::OnMatchStateChanged(EMatchState State)
{
    if (State == EMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
        bShowMouseCursor = true;
    }
}

void ANicePlayerController::OnMuteSound()
{
    if (!GetWorld()) return;

    const auto GameInstance = GetWorld()->GetGameInstance<UNiceGameInstance>();
    if (!GameInstance) return;

    GameInstance->ToggleVolume();
}
