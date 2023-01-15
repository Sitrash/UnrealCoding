// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NiceImmersive/CoreTypes.h"
#include "NiceImmersiveHUD.generated.h"

UCLASS()
class ANiceImmersiveHUD : public AHUD
{
    GENERATED_BODY()

public:
    ANiceImmersiveHUD();

    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EMatchState, UUserWidget*> GameWidgets;

    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;

    void DrawCrosshair();

    void OnMatchStateChanged(EMatchState State);
};
