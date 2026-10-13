// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomMovieSceneTrackEditor.h"
#include "ISequencerModule.h"
#include "SequencerChannelInterface.h"
#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"

#define LOCTEXT_NAMESPACE "FCustomMovieSceneTrackEditor"

#if 0
#include "GameplayTag/MovieSceneGameplayTagSection.h"
TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params)
{
	const TMovieSceneExternalValue<FText>* ExternalValue = Channel.GetExtendedEditorData();
	if (!ExternalValue)
	{
		return SNullWidget::NullWidget;
	}

	TSequencerKeyEditor<FMovieSceneTextChannel, FText> KeyEditor(
		Params.ObjectBindingID, Channel,
		Params.OwningSection, Params.Sequencer, Params.PropertyBindings, ExternalValue->OnGetExternalValue
	);
	KeyEditor.SetOwningObject(Cast<UMovieSceneSignedObject>(Params.OwningObject));

	return SNew(STextCurveKeyEditor, KeyEditor);
}
#endif

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