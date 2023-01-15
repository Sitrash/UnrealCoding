// Copyright

#include "NiceImmersiveCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WeaponComponent.h"
#include "Engine/EngineTypes.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryUI/InventoryWidget.h"
#include "Interactable/ChestInteract.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);
DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ANiceImmersiveCharacter::ANiceImmersiveCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    GetMesh()->SetupAttachment(FirstPersonCameraComponent);

    InteractionBox = CreateDefaultSubobject<UBoxComponent>("Interaction Box");
    InteractionBox->SetupAttachment(FirstPersonCameraComponent);

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("Health Text Component");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    DragHand = CreateDefaultSubobject<UBoxComponent>("DragHand");
    DragHand->SetupAttachment(FirstPersonCameraComponent);

    ForbidenCrouch = CreateDefaultSubobject<UBoxComponent>("ForbidenCrouch");
    ForbidenCrouch->SetupAttachment(GetCapsuleComponent());

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Weapon Component");
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void ANiceImmersiveCharacter::BeginPlay()
{
    Super::BeginPlay();

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &ANiceImmersiveCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ANiceImmersiveCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ANiceImmersiveCharacter::OnGroundLanded);

    Widget = CreateWidget<UInventoryWidget>(GetController<APlayerController>(), InventoryWidget);
}

void ANiceImmersiveCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    InteractionOverlapping();
}

void ANiceImmersiveCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ANiceImmersiveCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ANiceImmersiveCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Crouched", IE_Pressed, this, &ANiceImmersiveCharacter::Crouched);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ANiceImmersiveCharacter::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ANiceImmersiveCharacter::SprintEnd);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UWeaponComponent::Reload);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("UnEquip", IE_Pressed, WeaponComponent, &UWeaponComponent::UnEquip);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANiceImmersiveCharacter::OnFire);
    PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ANiceImmersiveCharacter::OnAction);
    PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ANiceImmersiveCharacter::OnInventoryShow).bExecuteWhenPaused = true;

    // is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("TurnRate", this, &ANiceImmersiveCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ANiceImmersiveCharacter::LookUpAtRate);
}

void ANiceImmersiveCharacter::MoveForward(float Value)
{
    IsMovingForward = Value > 0.0f;
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}
void ANiceImmersiveCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}
void ANiceImmersiveCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void ANiceImmersiveCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANiceImmersiveCharacter::Crouched()
{
    if (GetCharacterMovement()->IsFalling()) return;

    if (!IsCrouch)
    {
        GetCapsuleComponent()->SetCapsuleHalfHeight(33.0f, false);
        IsCrouch = true;
        GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    }
    else
    {
        TArray<AActor*> OverActors;
        ForbidenCrouch->GetOverlappingActors(OverActors);
        for (const auto& OverActor : OverActors)
        {
            if (OverActor)
            {
                OverActors.Empty();
                return;
            }
        }
        GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
        IsCrouch = false;
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    }
}

bool ANiceImmersiveCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}
void ANiceImmersiveCharacter::Sprint()
{
    WantsToRun = true;
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}
void ANiceImmersiveCharacter::SprintEnd()
{
    WantsToRun = false;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ANiceImmersiveCharacter::OnAction()
{
    if (Interface) Interface->InteractWithMe(this);
}

void ANiceImmersiveCharacter::OnFire()
{
    if (bHolding)
    {
        CanThrow = true;
        Interface->InteractWithMe(this);
    }
}

//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

void ANiceImmersiveCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ANiceImmersiveCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ANiceImmersiveCharacter::OnGroundLanded(const FHitResult& Hit2)
{
    const auto FallVecocityZ = -GetCharacterMovement()->Velocity.Z;

    if (FallVecocityZ < LandedDamageVelocity.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVecocityZ);
    TakeDamage(FinalDamage, FPointDamageEvent{}, nullptr, nullptr);
}

void ANiceImmersiveCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ANiceImmersiveCharacter::SubFuncForHolding(AActor* HoldingActor, bool IsHolding)
{
    bHolding = IsHolding;
    if (!bHolding)
    {
        WeaponComponent->CharacterGrabActor = nullptr;
        WeaponComponent->IsEquip() ? WeaponComponent->EquipWeapon(WeaponComponent->GetCurrentWeaponIndex()) : nullptr;
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    }
    else
    {
        WeaponComponent->CharacterGrabActor = HoldingActor;
        WeaponComponent->UnEquipForDrop();
        GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    }
}

float ANiceImmersiveCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ANiceImmersiveCharacter::InteractionOverlapping()
{
    TArray<AActor*> OverlappingActors;
    InteractionBox->GetOverlappingActors(OverlappingActors);

    if (OverlappingActors.Num() == 0)
    {
        if (Interface)
        {
            Interface->HideInteractionWidget();
            Interface = nullptr;
        }
        return;
    }

    AActor* ClosestActor = OverlappingActors[0];

    for (auto CurrentActor : OverlappingActors)
    {
        if (GetDistanceTo(CurrentActor) < GetDistanceTo(ClosestActor))
        {
            ClosestActor = CurrentActor;
        }
    }

    if (Interface)
    {
        Interface->HideInteractionWidget();
    }

    Interface = Cast<IInteractionInterface>(ClosestActor);

    if (Interface)
    {
        Interface->ShowInteractionWidget();
    }
}

void ANiceImmersiveCharacter::OnInventoryShow()
{
    if (!CanOpenInventory) return;
    const auto Ctrl = GetController<APlayerController>();

    if (!IsShowingInventory)
    {
        Ctrl->SetMouseLocation(XCoord, YCoord);
        Widget->AddToViewport();
        IsShowingInventory = true;
    }
    else
    {
        Ctrl->GetMousePosition(XCoord, YCoord);
        Widget->CloseInventory();
        IsShowingInventory = false;
    }
}