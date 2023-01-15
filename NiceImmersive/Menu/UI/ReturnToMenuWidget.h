// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMenuWidget.generated.h"

class UButton;

UCLASS()
class NICEIMMERSIVE_API UReturnToMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ReturnToMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnReturnToMenu();
};
