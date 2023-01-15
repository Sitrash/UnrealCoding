// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/NiceImmersiveCharacter.h"
#include "Components/BoxComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/HealthComponent.h"
#include "NPCCharacter.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
class UDamageType;
class AController;
class UHealthComponent;
class UTextRenderComponent;
class ABaseWeapon;

UCLASS(Blueprintable, BlueprintType, config=Game)
class ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Character)
	USkeletalMeshComponent* NPCMesh;

public:
	
	ANPCCharacter();

	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetNPCMesh() const { return NPCMesh; }

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float LifeSpanDeath = 5.0f;



protected:
	
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;


private:


	void OnDeath();
	void OnHealthChanged(float Health);

	void SpawnWeapon();

};
