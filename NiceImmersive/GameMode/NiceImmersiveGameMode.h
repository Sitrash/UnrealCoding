// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NiceImmersive/CoreTypes.h"
#include "NiceImmersiveGameMode.generated.h"

class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegateSignature, bool, IsPaused);

UCLASS()
class ANiceImmersiveGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANiceImmersiveGameMode();

    FOnMatchStateChangedSigntarure OnMatchStateChanged;

    virtual void StartPlay() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    void Killed(AController* KillerController, AController* VictimController);

    FGameData GetGameData() const { return GameData; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

    void RespawnRequest(AController* Controller);

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

    virtual bool ClearPause() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    FGameData GameData;

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegateSignature OnGamePause;

protected:
    UPROPERTY(EditAnywhere, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditAnywhere, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    // FGameData GameData;

private:
    EMatchState MatchState = EMatchState::WaitingToStart;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();

    void SetMatchState(EMatchState State);

    void StopAllFire();
};
