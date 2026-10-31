// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomMovieSceneTrackEditor.h"
#include "ISequencerModule.h"
#include "SequencerChannelInterface.h"

#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"
#include "CustomClipboardTypes.h"

#define LOCTEXT_NAMESPACE "FCustomMovieSceneTrackEditor"

void FCustomMovieSceneTrackEditor::StartupModule()
{
	if (GIsEditor)
	{
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

		// GameplayTag
		GameplayTagPropertyTrackCreateEditorHandle = SequencerModule.RegisterPropertyTrackEditor<FGameplayTagPropertyTrackEditor>();
		SequencerModule.RegisterChannelInterface<FMovieSceneGameplayTagChannel>();
	}
}

void FCustomMovieSceneTrackEditor::ShutdownModule()
{
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

	// GameplayTag
	SequencerModule.UnRegisterTrackEditor(GameplayTagPropertyTrackCreateEditorHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomMovieSceneTrackEditor, CustomMovieSceneTrackEditor)