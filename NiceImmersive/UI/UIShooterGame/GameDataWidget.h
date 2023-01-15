// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameDataWidget.generated.h"

class ANiceImmersiveGameMode;
class ANicePlayerState;

UCLASS()
class NICEIMMERSIVE_API UGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    int32 GetKillsNum() const;

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentRoundNum() const;

    UFUNCTION(BlueprintCallable)
    int32 GetTotalRoundsNum() const;

    UFUNCTION(BlueprintCallable)
    int32 GetRoundSecondsRemaining() const;

private:
    ANiceImmersiveGameMode* GetNiceGameMode() const;
    ANicePlayerState* GetNicePlayerState() const;
};
