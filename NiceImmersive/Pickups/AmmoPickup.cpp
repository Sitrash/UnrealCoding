// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "UtilsNiceImmersive.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool AAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
