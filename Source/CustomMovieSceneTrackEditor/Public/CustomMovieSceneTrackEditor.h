// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameplayTagContainer.h"
#include "MovieSceneClipboard.h"

class FCustomMovieSceneTrackEditor : public IModuleInterface
{
public:

	//~Begin IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End IModuleInterface interface

private:
	FDelegateHandle GameplayTagPropertyTrackCreateEditorHandle;
};
