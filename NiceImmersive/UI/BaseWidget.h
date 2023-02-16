// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiceImmersive/Interfaces/InteractionInterface.h"
#include "BaseWidget.generated.h"

class ANiceImmersiveCharacter;
class APlayerController;

UCLASS()
class NICEIMMERSIVE_API UBaseWidget : public UUserWidget, public IInteractionInterface
{
    GENERATED_BODY()

public:

protected:
    virtual void NativeOnInitialized() override;

private:

};
