// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/ReturnToMenuWidget.h"
#include "GameMode/NiceGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogReturnToMenuWidget, All, All);

void UReturnToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ReturnToMenuButton)
    {
        ReturnToMenuButton->OnClicked.AddDynamic(this, &UReturnToMenuWidget::OnReturnToMenu);
    }
}

void UReturnToMenuWidget::OnReturnToMenu()
{

    if (!GetWorld()) return;

    const auto NiceGameInstance = GetWorld()->GetGameInstance<UNiceGameInstance>();
    if (!NiceGameInstance) return;

    if (NiceGameInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogReturnToMenuWidget, Error, TEXT(" Menu level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, NiceGameInstance->GetMenuLevelName());
}
