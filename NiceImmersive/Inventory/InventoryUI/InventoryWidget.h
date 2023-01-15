// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWrapBox;
class UInventoryItemWidget;
class UInventoryComponent;

UCLASS()
class NICEIMMERSIVE_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void CloseInventory();

protected:
    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInventoryItemWidget> InventoryItemClass;

    UInventoryComponent* InventoryReference;

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateInventory();
};
