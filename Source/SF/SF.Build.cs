// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SF : ModuleRules
{
	public SF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
			"SF"
		});

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore", 
			"EnhancedInput",
			"UMG",
			"Slate",

			"NetCore",

			"NavigationSystem", 


			// OnlineSystem
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemNull",

			// AI
			"AIModule",
		});
	}
}
