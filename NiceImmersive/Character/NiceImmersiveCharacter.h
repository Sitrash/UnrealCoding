// Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Components/HealthComponent.h"
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
class AChestInteract;
class USoundCue;
class UInventoryWidget;

UCLASS(Blueprintable, BlueprintType, config = Game)
class ANiceImmersiveCharacter : public ACharacter, public IGenericTeamAgentInterface
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

    UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWeaponComponent* WeaponComponent;

    UBoxComponent* GetDragHand() { return DragHand; }
    bool CanThrowing() { return CanThrow; }
    void SetThrowing(bool b) { CanThrow = b; }
    bool GetHolding() { return bHolding; }
    void SubFuncForHolding(AActor* HoldingActor, bool IsHolding);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    void SetPlayerColor(const FLinearColor& Color);

    bool CanOpenInventory = true;
    bool IsShowingInventory = false;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TSubclassOf<UUserWidget> InventoryWidget;

    void OnAction();
    void OnFire();

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
    IInteractionInterface* Interface = nullptr;

    bool WantsToRun = false;
    bool IsMovingForward = false;
    bool IsCrouch = false;

    bool bHolding = false;
    bool CanThrow = false;

    void OnHealthChanged(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit2);

    void InteractionOverlapping();

    UInventoryWidget* Widget = nullptr;

    UFUNCTION(BlueprintCallable)
    void OnInventoryShow();

    float XCoord = 200.0f;
    float YCoord = 200.0f;
};
