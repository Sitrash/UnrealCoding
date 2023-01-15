// Fill out your copyright notice in the Description page of Project Settings.

#include "NiceImmersiveBaseAnimNotify.h"

void UNiceImmersiveBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}