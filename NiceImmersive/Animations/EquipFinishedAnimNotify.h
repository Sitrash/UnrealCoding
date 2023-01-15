// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiceImmersiveBaseAnimNotify.h"
#include "EquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class NICEIMMERSIVE_API UEquipFinishedAnimNotify : public UNiceImmersiveBaseAnimNotify
{
    GENERATED_BODY()
};
