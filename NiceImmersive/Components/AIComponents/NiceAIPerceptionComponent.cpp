// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceAIPerceptionComponent.h"
#include "AI/NiceAIController.h"
#include "UtilsNiceImmersive.h"
#include "Components/HealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "DrawDebugHelpers.h"

AActor* UNiceAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() == 0) return nullptr;
    }

    /*GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PercieveActors);

    for (const auto PercieveActor : PercieveActors)
    {
        UE_LOG(LogTemp, Display, TEXT("I'm hear you: %s"), *PercieveActor->GetName());
        const FAISenseID HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>();
        if (GetSenseConfig(HearingSenseID))
        {
            const auto HearingInfo = GetFreshestTrace(HearingSenseID);
            if (HearingInfo && HasActiveStimulus(*HearingInfo->Target, HearingSenseID))
            {
                const auto Location = HearingInfo->GetStimulusLocation(HearingSenseID);
                DrawDebugSphere(GetWorld(), Location, 100.0f, 24, FColor::Yellow);
                break;
            }
        }
    }*/

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto PercieveActor : PercieveActors)
    {

        const auto HealthComponent = AUtilsNiceImmersive::GetNicePlayerComponent<UHealthComponent>(PercieveActor);

        const auto PercievePawn = Cast<APawn>(PercieveActor);
        const auto AreEnemies = PercievePawn && AUtilsNiceImmersive::AreEnemies(Controller, PercievePawn->Controller);

        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}

FVector UNiceAIPerceptionComponent::GetClosestVector()
{
    TArray<AActor*> PercieveActors;
    FVector Loc;
    auto Niceu = Loc.ZeroVector;

    GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PercieveActors);

    for (auto PercieveActor : PercieveActors)
    {
        const FAISenseID HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>();
        if (GetSenseConfig(HearingSenseID))
        {
            const auto HearingInfo = GetFreshestTrace(HearingSenseID);
            if (HearingInfo && HasActiveStimulus(*HearingInfo->Target, HearingSenseID))
            {
                auto Location = HearingInfo->GetStimulusLocation(HearingSenseID);
                DrawDebugSphere(GetWorld(), Location, 100.0f, 24, FColor::Yellow);
                Niceu = Location;
                break;
            }
        }
    }
    if (Loc == FVector(0, 0, 0))
    {
        return FVector(0, 0, 0);
    }
    else
    {
        return Niceu;
    }
}