// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"
#include "Components/HealthComponent.h"
#include "UtilsNiceImmersive.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool AHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(PlayerPawn);
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}
