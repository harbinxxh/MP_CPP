// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MP_CPP : ModuleRules
{
	public MP_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MP_CPP",
			"MP_CPP/Variant_Platforming",
			"MP_CPP/Variant_Platforming/Animation",
			"MP_CPP/Variant_Combat",
			"MP_CPP/Variant_Combat/AI",
			"MP_CPP/Variant_Combat/Animation",
			"MP_CPP/Variant_Combat/Gameplay",
			"MP_CPP/Variant_Combat/Interfaces",
			"MP_CPP/Variant_Combat/UI",
			"MP_CPP/Variant_SideScrolling",
			"MP_CPP/Variant_SideScrolling/AI",
			"MP_CPP/Variant_SideScrolling/Gameplay",
			"MP_CPP/Variant_SideScrolling/Interfaces",
			"MP_CPP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
