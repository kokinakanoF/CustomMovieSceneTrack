// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MovieSceneClipboard.h"

/**
 * ここにキーの型を登録しておくことで、ISequencerModule::RegisterChannelInterface()を使って
 * チャネルを登録することができるようになります。
 */
namespace MovieSceneClipboard
{
	// GameplayTag
	template<> inline FName GetKeyTypeName<FGameplayTag>()
	{
		return "GameplayTag";
	}
}