// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MovieSceneClipboard.h"

/**
 * Registering key types here makes it possible to register channels using
 * ISequencerModule::RegisterChannelInterface().
 */
namespace MovieSceneClipboard
{
	// GameplayTag
	template<> inline FName GetKeyTypeName<FGameplayTag>()
	{
		return "GameplayTag";
	}
}