// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiceImmersive/UI/BaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "ChestWidget.generated.h"

class UWrapBox;
class UChestItemWidget;
class UInventoryComponent;
class UButton;
class AChestInteract;
class USoundBase;

UCLASS()
class NICEIMMERSIVE_API UChestWidget : public UBaseWidget
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    USoundBase* OpenInventorySound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    USoundBase* CloseInventorySound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FName PauseGame;

    UInventoryComponent* InventoryReference;

    FOnInputAction InputAction;

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateInventory();

private:
    //float XCoord = 650.0f;
    //float YCoord = 300.0f;
};
