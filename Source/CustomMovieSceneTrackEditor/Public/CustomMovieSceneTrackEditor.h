// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameplayTagContainer.h"
#include "MovieSceneClipboard.h"

namespace MovieSceneClipboard
{
	template<> inline FName GetKeyTypeName<FGameplayTag>()
	{
		static FName Name("GameplayTag");
		return Name;
	}
}

class FCustomMovieSceneTrackEditor : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FDelegateHandle GameplayTagPropertyTrackCreateEditorHandle;
};
