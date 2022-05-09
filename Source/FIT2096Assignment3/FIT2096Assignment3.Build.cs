// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FIT2096Assignment3 : ModuleRules
{
	public FIT2096Assignment3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
