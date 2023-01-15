// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NiceImmersive : ModuleRules
{
	public NiceImmersive(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "HeadMountedDisplay", "PhysicsCore", "UMG", "Slate", "SlateCore", "NavigationSystem", "AIModule", "GameplayTasks" });

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
	}
}
