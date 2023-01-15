// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ANiceImmersiveCharacter;

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
    GENERATED_BODY()
};

class NICEIMMERSIVE_API IInteractionInterface
{
    GENERATED_BODY()

public:
    virtual void InteractWithMe(ANiceImmersiveCharacter* Pawn);
    virtual void ShowInteractionWidget();
    virtual void HideInteractionWidget();
};
