// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * カスタマイズされたシーケンサートラックを使えるようにするためのモジュール（ランタイム用） 
 */
class FCustomMovieSceneTrack : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
