// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestWidget.generated.h"

class UWrapBox;
class UChestItemWidget;
class UInventoryComponent;
class UButton;
class AChestInteract;

UCLASS()
class NICEIMMERSIVE_API UChestWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void CloseInventory();

    AChestInteract* ChestActor = nullptr;

    bool LastNum = false;

protected:
    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryBox;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UChestItemWidget> ItemWidgetClass;

    UInventoryComponent* InventoryReference;

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateInventory();

private:
    float XCoord = 650.0f;
    float YCoord = 300.0f;
};
