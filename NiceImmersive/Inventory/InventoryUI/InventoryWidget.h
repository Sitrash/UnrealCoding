// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiceImmersive/UI/BaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWrapBox;
class UInventoryItemWidget;
class UInventoryComponent;
class USoundBase;

UCLASS()
class NICEIMMERSIVE_API UInventoryWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void CloseInventory();

    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryBox;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInventoryItemWidget> InventoryItemClass;

    UInventoryComponent* InventoryReference;

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateInventory();

    FOnInputAction InputAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FName PauseGame;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    USoundBase* OpenInventorySound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    USoundBase* CloseInventorySound;
};