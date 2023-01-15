// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiceImmersiveGameMode.h"
#include "UI/NiceImmersiveHUD.h"
#include "NiceImmersiveCharacter.h"
#include "NicePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "AIController.h"
#include "NicePlayerState.h"
#include "UtilsNiceImmersive.h"
#include "Components/RespawnComponent.h"
#include "EngineUtils.h"
#include "Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ANiceImmersiveGameMode::ANiceImmersiveGameMode() : Super()
{
    // static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
    // DefaultPawnClass = PlayerPawnClassFinder.Class;

    DefaultPawnClass = ANiceImmersiveCharacter::StaticClass();
    PlayerControllerClass = ANicePlayerController::StaticClass();
    HUDClass = ANiceImmersiveHUD::StaticClass();
    PlayerStateClass = ANicePlayerState::StaticClass();
}

void ANiceImmersiveGameMode::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();

    SetMatchState(EMatchState::InProgress);
}

UClass* ANiceImmersiveGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ANiceImmersiveGameMode::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayerNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto NiceAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(NiceAIController);
    }
}

void ANiceImmersiveGameMode::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ANiceImmersiveGameMode::GameTimerUpdate, 1.0f, true);
}

void ANiceImmersiveGameMode::GameTimerUpdate()
{

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ANiceImmersiveGameMode::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ANiceImmersiveGameMode::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ANiceImmersiveGameMode::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ANicePlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ANiceImmersiveGameMode::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());

    return GameData.DefaultTeamColor;
}

void ANiceImmersiveGameMode::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<ANiceImmersiveCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<ANicePlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ANiceImmersiveGameMode::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ANicePlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ANicePlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void ANiceImmersiveGameMode::LogPlayerInfo()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ANicePlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void ANiceImmersiveGameMode::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void ANiceImmersiveGameMode::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!RespawnAvailable) return;
    const auto RespawnComponent = AUtilsNiceImmersive::GetNicePlayerComponent<URespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ANiceImmersiveGameMode::GameOver()
{
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(EMatchState::GameOver);
}

void ANiceImmersiveGameMode::SetMatchState(EMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}

void ANiceImmersiveGameMode::StopAllFire()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(Pawn);
        if (!WeaponComponent) continue;

        WeaponComponent->StopFire();
    }
}

bool ANiceImmersiveGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

    if (PauseSet)
    {
        OnGamePause.Broadcast(true);
        StopAllFire();
        SetMatchState(EMatchState::Pause);
    }

    return PauseSet;
}

bool ANiceImmersiveGameMode::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        OnGamePause.Broadcast(false);
        SetMatchState(EMatchState::InProgress);
    }
    return PauseCleared;
}
