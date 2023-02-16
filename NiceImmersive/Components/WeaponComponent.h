// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiceImmersive/CoreTypes.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;

DECLARE_MULTICAST_DELEGATE(FStopFireEquipSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NICEIMMERSIVE_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();

    void EquipWeapon(int32 WeaponIndex);
    void UnEquipForDrop();
    void UnEquip();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<ABaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<ABaseWeapon> WeaponType);

    int32 GetCurrentWeaponIndex() { return CurrentWeaponIndex; }
    void SetCurrentWeaponIndex(int32 IndexWeapon) { CurrentWeaponIndex = IndexWeapon; }

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    bool IsHoldingWeapon() { return HoldingWeapon; }
    bool IsEquip() const { return Equip; }

    UPROPERTY()
    ABaseWeapon* CurrentWeapon = nullptr;

    AActor* CharacterGrabActor = nullptr;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanEquip() const;
    bool CanFire() const;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY()
    TArray<ABaseWeapon*> Weapons;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* FireAnimMontage;

    bool HoldingWeapon;

    int32 CurrentWeaponIndex = 0;

    FStopFireEquipSignature OnUnEquipStopFire;

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool Equip = false;

    void InitAnimations();
    void SpawnWeapons();

    void PlayAnimMontage(UAnimMontage* Animation);
    void StopAnimMontage(UAnimMontage* Animation);

    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanReload() const;

    void AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    void OnEmptyClip(ABaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();
};
