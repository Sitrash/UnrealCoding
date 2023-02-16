// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UItem;
class UTextBlock;
class UButton;
class UImage;
class UCanvasPanel;

UCLASS()
class NICEIMMERSIVE_API UInventoryItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UItem* ItemReference;

    void SetAllNice(UItem* ItemRef);

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UButton* UseButton;

    UPROPERTY(meta = (BindWidget))
    UButton* Use;

    UPROPERTY(meta = (BindWidget))
    UButton* Drops;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UCanvasPanel* UsesButtonsCanvas;

    UFUNCTION()
    void ShowSubWidget();

    UFUNCTION()
    void OnUseItem();

    UFUNCTION(BlueprintCallable)
    void OnDropItem();

    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
