// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Weapons/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	


	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>("NPCMesh");
	NPCMesh->SetOnlyOwnerSee(false);
	NPCMesh->SetupAttachment(GetCapsuleComponent());
	NPCMesh->bCastDynamicShadow = false;
	NPCMesh->CastShadow = true;
	NPCMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, -0.0f));
	NPCMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("Health Text Component");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}



void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ANPCCharacter::OnDeath);
	//HealthComponent->OnHealthChanged.AddUObject(this, &ANPCCharacter::OnHealthChanged);
	
	SpawnWeapon();

	//check(HealthData.MaxHealth > 0.0f);
	//Health = HealthData.MaxHealth;

	//OnTakeAnyDamage.AddDynamic(this, &ANPCCharacter::OnTakeAnyDamageRecieved);
	
}

void ANPCCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}



void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ANPCCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());
	
	PlayAnimMontage(DeathAnimMontage, 1, NAME_None);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanDeath);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ANPCCharacter::SpawnWeapon()
{
	if (!GetWorld()) return;

	const auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
		UE_LOG(BaseCharacterLog, Display, TEXT("DONE!"));
	}
}






//float ANPCCharacter::GetHealthPercent() const
//{
//	return Health / HealthData.MaxHealth;
//}
/*void ANPCCharacter::OnAnyDamageRecieved(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	const auto IsAlive = [&]() { return Health > 0.0f; };

	if (Damage <= 0.0f || !IsAlive()) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, HealthData.MaxHealth);
	if(IsAlive())
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ANPCCharacter::OnHealing, HealthData.HealRate, true, -1.0f);
	}
	else
	{
		OnDeath();
	}
}
void ANPCCharacter::OnHealing()
{
	Health = FMath::Clamp(Health + HealthData.HealModifier, 0.0f, HealthData.MaxHealth);
	if(FMath::IsNearlyEqual(Health, HealthData.MaxHealth))
	{
		Health = HealthData.MaxHealth;
		GetWorldTimerManager().ClearTimer(HealTimerHandle);	
	}
}
void ANPCCharacter::OnDeath()
{

	UAnimInstance* AnimInstance = NPCMesh->GetAnimInstance();
	AnimInstance->Montage_Play(DeathAnim, 3.f);


	GetWorldTimerManager().ClearTimer(HealTimerHandle);	

	

	check(GetCharacterMovement());
	check(GetCapsuleComponent());
	check(GetNPCMesh());

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetNPCMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetNPCMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	GetNPCMesh()->SetSimulatePhysics(true);
	

	SetLifeSpan(HealthData.LifeSpan);
}

    //HEALTH AND DAMAGE FUNCTION!!!!!!!!! END
	*/
	
