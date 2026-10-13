// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class CustomMovieSceneTrack : ModuleRules
{
	public CustomMovieSceneTrack(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
				// For UI
                "Slate",
                "SlateCore",
                "UMG",
            }
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "MovieScene",
				"MovieSceneTracks",
				"GameplayTags"
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				//"CoreUObject",
				//"Engine",
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
