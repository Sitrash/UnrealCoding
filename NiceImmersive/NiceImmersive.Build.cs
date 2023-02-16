// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NiceImmersive : ModuleRules
{
    public NiceImmersive(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "HeadMountedDisplay",
            "PhysicsCore", "UMG", "Slate", "SlateCore", "NavigationSystem", "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.AddRange(new string[]
        {
            "NiceImmersive/",
            "NiceImmersive/Components",
            "NiceImmersive/Weapons",
            "NiceImmersive/UI",
            "NiceImmersive/Animations",
            "NiceImmersive/Pickups",
            "NiceImmersive/AI",
            "NiceImmersive/AI/Services",
            "NiceImmersive/AI/Decorators",
            "NiceImmersive/AI/Tasks",
            "NiceImmersive/AI/EQS",
            "NiceImmersive/Menu/",
            "NiceImmersive/Menu/UI",
            "NiceImmersive/Inventory/InventoryUI",
            "NiceImmersive/Inventory/Items",
            "NiceImmersive/Components/AIComponents",
            "NiceImmersive/Settings",
            "NiceImmersive/Settings/UISettings",
            "NiceImmersive/Character",
            "NiceImmersive/UI/UIShooterGame",
            "NiceImmersive/Weapons/DamageActors",
            "NiceImmersive/GameMode",
            "NiceImmersive/Interactable",
            "NiceImmersive/Interfaces",
            "NiceImmersive/Tests"


        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
