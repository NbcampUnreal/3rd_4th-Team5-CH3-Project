// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CH3Project : ModuleRules
{
	public CH3Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "GameplayTasks", "Niagara", "UMG"});	}
}
