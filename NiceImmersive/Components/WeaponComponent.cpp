// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/EquipFinishedAnimNotify.h"
#include "Animations/NiceImmersiveBaseAnimNotify.h"
#include "Animations/ReloadAnimNotify.h"
#include "AnimUtils.h"
#include "CoreTypes.h"
#include "Weapons/LauncherWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    // checkf(WeaponData.Num() == WeaponNum, TEXT("Character can hold only %i weapon items"), WeaponNum);

    InitAnimations();
    CurrentWeaponIndex = 0;
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
    OnUnEquipStopFire.AddUObject(this, &UWeaponComponent::StopFire);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{

    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    // CurrrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData =
        WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
    CurrentWeapon->SetActorHiddenInGame(false);

    HoldingWeapon = true;
    Equip = true;
}

void UWeaponComponent::UnEquipForDrop()
{
    if (!Equip) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);

    PlayAnimMontage(EquipAnimMontage);
    OnUnEquipStopFire.Broadcast();
    HoldingWeapon = false;
}

void UWeaponComponent::UnEquip()
{
    if (ReloadAnimInProgress || EquipAnimInProgress) return;
    if (!CurrentWeapon) return;
    if (!Equip) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);

    PlayAnimMontage(EquipAnimMontage);
    OnUnEquipStopFire.Broadcast();
    Equip = false;
    HoldingWeapon = false;
}

void UWeaponComponent::StartFire()
{
    if (!CanFire())
    {
        return;
    }
    else
    {
        if (!IsHoldingWeapon())
        {
            EquipWeapon(CurrentWeaponIndex);
            SetCurrentWeaponIndex(CurrentWeaponIndex);
        }
        else
        {
            CurrentWeapon->StartFire();
            const auto LauncherWeapon = Cast<ALauncherWeapon>(CurrentWeapon);
            if (!LauncherWeapon && !CurrentWeapon->IsAmmoEmpty()) PlayAnimMontage(FireAnimMontage);
        }
    }
}

void UWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
    StopAnimMontage(FireAnimMontage);
}

void UWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void UWeaponComponent::StopAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->StopAnimMontage(Animation);
}

void UWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify zabil postavit'!"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UReloadAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify zabil postavit'!"));
            checkNoEntry();
        }

        ReloadFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
    }
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComp != Character->GetMesh()) return;

    EquipAnimInProgress = false;
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComp != Character->GetMesh()) return;

    ReloadAnimInProgress = false;
}

bool UWeaponComponent::CanFire() const
{
    return CurrentWeapon 
        && !EquipAnimInProgress
        && !ReloadAnimInProgress
        && !CharacterGrabActor;
}

bool UWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress
        && !ReloadAnimInProgress
        && !CharacterGrabActor;
}

bool UWeaponComponent::CanReload() const
{
    return CurrentWeapon 
        && !EquipAnimInProgress
        && !ReloadAnimInProgress
        && CurrentWeapon->CanReload()
        && !CharacterGrabActor;
}

void UWeaponComponent::Reload()
{
    ChangeClip();
}

void UWeaponComponent::OnEmptyClip(ABaseWeapon* AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon) return;

    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for (const auto Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void UWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
    CurrentWeapon->ChangeClip();
}

bool UWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool UWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool UWeaponComponent::TryToAddAmmo(TSubclassOf<ABaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

bool UWeaponComponent::NeedAmmo(TSubclassOf<ABaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}
