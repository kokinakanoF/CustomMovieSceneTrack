// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class CustomMovieSceneTrackEditor : ModuleRules
{
	public CustomMovieSceneTrackEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
        PrivateIncludePaths.AddRange(
            new string[] 
            {
                System.IO.Path.Combine(GetModuleDirectory("MovieSceneTools"), "Private"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
	            // For UI
                "Slate",
                "SlateCore",

				"UnrealEd",
                "GameplayTags",
                "GameplayTagsEditor",
				"Sequencer",
                "SequencerCore",
                "MovieScene",
				"MovieSceneTracks",
				"MovieSceneTools",
                "CustomMovieSceneTrack",
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
