// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersivePlayerHUDWidget.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "UtilsNiceImmersive.h"

void UNiceImmersivePlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UNiceImmersivePlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UNiceImmersivePlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UNiceImmersivePlayerHUDWidget::OnHealthChanged);
    }
}

void UNiceImmersivePlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}

float UNiceImmersivePlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool UNiceImmersivePlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UNiceImmersivePlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UNiceImmersivePlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool UNiceImmersivePlayerHUDWidget::IsPlayerSpecating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
