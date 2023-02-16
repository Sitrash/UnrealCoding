

#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NiceImmersive/GameMode/NiceImmersiveGameMode.h"
#include "Perception/AISense_Damage.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnTakeRadialDamage);
    }
}

void UHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void UHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
    class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
    AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    ApplyDamage(FinalDamage, InstigatedBy);
    UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());
    // StartPhysReaction(BoneName, ShotFromDirection);
}

void UHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
    FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
    UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
}

void UHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    PlayCameraShake();
    ReportDamageEvent(Damage, InstigatedBy);
}

void UHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool UHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);

    return true;
}

bool UHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());

    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;
    const auto GameMode = Cast<ANiceImmersiveGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}

void UHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

    UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(),
        GetOwner()->GetActorLocation());
}

float UHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character ||  //
        !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName))
        return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

    return DamageModifiers[PhysMaterial];
}

//
// void UHealthComponent::StartPhysReaction(const FName& BoneName, const FVector& ShotFromDirection)
//{
//	const auto Character = Cast<ACharacter>(GetOwner());
//
//	if (!CurrentBoneName.IsNone())
//	{
//		StopCurrentBoneSimulation();
//	}
//
//	CurrentHitBlend = 0.7f;  // how much 0..1
//	CurrentBoneName = BoneName;
//
//	const float ImpulseMagnitude = 500.0f;
//	Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(CurrentBoneName, true);
//	Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, CurrentHitBlend);
//	Character->GetMesh()->AddImpulseToAllBodiesBelow(ImpulseMagnitude * ShotFromDirection, CurrentBoneName, true);
//
//	const float TimerRate = 0.003f;
//	GetWorld()->GetTimerManager().ClearTimer(HitReactTimer);
//	GetWorld()->GetTimerManager().SetTimer(HitReactTimer, this, &UHealthComponent::UpdateSimulation, TimerRate, true);
//}
//
// void UHealthComponent::UpdateSimulation()
//{
//	CurrentHitBlend -= 0.01f;
//
//	if (CurrentHitBlend <= 0.0f)
//	{
//		GetWorld()->GetTimerManager().ClearTimer(HitReactTimer);
//		StopCurrentBoneSimulation();
//	}
//	else
//	{
//		const auto Character = Cast<ACharacter>(GetOwner());
//		Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, CurrentHitBlend);
//	}
//}
//
// void UHealthComponent::StopCurrentBoneSimulation()
//{
//	CurrentBoneName = NAME_None;
//	const auto Character = Cast<ACharacter>(GetOwner());
//	Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(CurrentBoneName, false);
//	Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, 0.0f);
//}