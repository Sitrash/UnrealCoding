// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class ANiceImmersiveCharacter;
class UBaseWidget;
class UInventoryWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NICEIMMERSIVE_API UActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UActionComponent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UUserWidget> ChestWidgetClass;

    void OnInventoryShow();
    bool ForbidShowInventory();
    void OnAction();
    void OnFire();

    bool CanThrowing() { return CanThrow; }
    void SetThrowing(bool b) { CanThrow = b; }

    bool GetHolding() { return bHolding; }
    void SubFuncForHolding(AActor* HoldingActor, bool IsHolding);

    bool CanOpenInventory = true;
    bool IsShowingInventory = false;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UUserWidget> InventoryWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UUserWidget> BaseWidgetClass;

private:
    ANiceImmersiveCharacter* Pawn = nullptr;
    UBaseWidget* MyBaseWidget = nullptr;
    UInventoryWidget* Widget = nullptr;

    bool bHolding = false;
    bool CanThrow = false;
};
