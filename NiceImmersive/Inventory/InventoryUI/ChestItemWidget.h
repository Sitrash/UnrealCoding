// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestItemWidget.generated.h"

class UItem;
class UTextBlock;
class UButton;
class UImage;
class AChestInteract;
class UChestWidget;

UCLASS()
class NICEIMMERSIVE_API UChestItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UItem* ItemReference;

    void SetAllNice(UItem* ItemRef);

    AChestInteract* Chest;

    UChestWidget* ParentWidget;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UButton* UseButton;

    UFUNCTION()
    void OnAddItems();

    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;
};
