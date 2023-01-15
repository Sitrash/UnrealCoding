// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubWidget.generated.h"

class UItem;
class UButton;

UCLASS()
class NICEIMMERSIVE_API USubWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UItem* ItemRef;

    UPROPERTY(meta = (BindWidget))
    UButton* Use;

    UPROPERTY(meta = (BindWidget))
    UButton* Drops;

protected:
    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnUseItem();

    UFUNCTION()
    void OnDropItem();
};
