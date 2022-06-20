// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SunriseProject : ModuleRules
{
	public SunriseProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
	}
}
