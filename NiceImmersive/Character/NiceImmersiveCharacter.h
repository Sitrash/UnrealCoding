// Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiceImmersive/Interfaces/InteractionInterface.h"
#include "NiceImmersive/Components/HealthComponent.h"
#include "NiceImmersiveCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UDamageType;
class AController;
class UHealthComponent;
class UTextRenderComponent;
class UWeaponComponent;
class UInventoryComponent;
class USoundCue;
class UActionComponent;
class UCameraShakeBase;
class USpringArmComponent;

UCLASS(Blueprintable, BlueprintType, config = Game)
class ANiceImmersiveCharacter : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

public:
    ANiceImmersiveCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float LifeSpanDeath = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
    FName MaterialColorName = "Paint Color";

    UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCameraComponent; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UActionComponent* ActionComponent;

    UBoxComponent* GetDragHand() { return DragHand; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    void SetPlayerColor(const FLinearColor& Color);

    IInteractionInterface* Interface = nullptr;

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
    virtual void BeginPlay();
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* InteractionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* DragHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* ForbidenCrouch;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> WalkCameraShake;

    void MoveForward(float Val);
    void MoveRight(float Val);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    void Crouched();

    void Sprint();
    void SprintEnd();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    virtual void OnDeath();

private:
    bool WantsToRun = false;
    bool IsMovingForward = false;
    bool IsCrouch = false;

    void OnHealthChanged(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit2);

    void InteractionOverlapping();

    APlayerController* CharController = nullptr;
};
