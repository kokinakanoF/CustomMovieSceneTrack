// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomMovieSceneTrackEditor.h"
#include "ISequencerModule.h"
#include "SequencerChannelInterface.h"
#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"

#define LOCTEXT_NAMESPACE "FCustomMovieSceneTrackEditor"

void FCustomMovieSceneTrackEditor::StartupModule()
{
	if (GIsEditor)
	{
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

		GameplayTagPropertyTrackCreateEditorHandle = SequencerModule.RegisterPropertyTrackEditor<FGameplayTagPropertyTrackEditor>();

		//TODO:ここを有効化するとエラーになるので対応！
		SequencerModule.RegisterChannelInterface<FMovieSceneGameplayTagChannel>();
	}
}

void FCustomMovieSceneTrackEditor::ShutdownModule()
{
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(GameplayTagPropertyTrackCreateEditorHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomMovieSceneTrackEditor, CustomMovieSceneTrackEditor)