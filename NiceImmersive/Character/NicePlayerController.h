// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NiceImmersive/CoreTypes.h"
#include "NicePlayerController.generated.h"

class URespawnComponent;
class UInventoryWidget;

UCLASS()
class NICEIMMERSIVE_API ANicePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ANicePlayerController();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    URespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPauseGame();
    void OnMatchStateChanged(EMatchState State);
    void OnMuteSound();
};
