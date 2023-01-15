// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/NiceSetting.h"

void UNiceSetting::SetName(const FText& InName)
{
    Name = InName;
}

void UNiceSetting::SetOptions(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UNiceSetting::GetCurrentOption() const
{
    const int32 CurrentValue = GetCurrentValue();
    const auto Option = Options.FindByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
    if (!Option)
    {
        UE_LOG(LogTemp, Error, TEXT("Option doesn't exist"));
        return FSettingOption{};
    }
    return *Option;
}

FText UNiceSetting::GetName() const
{
    return Name;
}

void UNiceSetting::AddGetter(TFunction<int32()> Func)
{
    Getter = Func;
}

void UNiceSetting::AddSetter(TFunction<void(int32)> Func)
{
    Setter = Func;
}

void UNiceSetting::ApplyNextOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
    SetCurrentValue(Options[NextIndex].Value);
}

void UNiceSetting::ApplyPrevOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 PrevIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(Options[PrevIndex].Value);
}

int32 UNiceSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogTemp, Error, TEXT("Getter func is not set for %s"), *Name.ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UNiceSetting::SetCurrentValue(int32 Value)
{
    if (!Setter)
    {
        UE_LOG(LogTemp, Error, TEXT("Setter func is not set for %s"), *Name.ToString());
        return;
    }
    Setter(Value);
}

int32 UNiceSetting::GetCurrentIndex() const
{
    const int32 CurrentValue = GetCurrentValue();
    return Options.IndexOfByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
}
