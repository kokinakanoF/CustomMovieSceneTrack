// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"

bool CanCreateKeyEditor(const FMovieSceneGameplayTagChannel* Channel)
{
	return true;
}

TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params)
{
	const TMovieSceneExternalValue<FGameplayTag>* ExternalValue = Channel.GetExtendedEditorData();
	if (!ExternalValue)
	{
		return SNullWidget::NullWidget;
	}

	TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag> KeyEditor(
		Params.ObjectBindingID, Channel,
		Params.OwningSection, Params.Sequencer, Params.PropertyBindings, ExternalValue->OnGetExternalValue
	);
	KeyEditor.SetOwningObject(Cast<UMovieSceneSignedObject>(Params.OwningObject));

	return SNew(SGameplayTagCurveKeyEditor, KeyEditor);
}




TSharedRef<ISequencerTrackEditor> FGameplayTagPropertyTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
{
	return MakeShared<FGameplayTagPropertyTrackEditor>(OwningSequencer);
}

void FGameplayTagPropertyTrackEditor::GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams
	, UMovieSceneSection* SectionToKey
	, FGeneratedTrackKeys& OutGeneratedKeys)
{
	// TODO:この処理が必要か確認
#if 0
	const FTextProperty* TextProperty = CastField<const FTextProperty>(PropertyChangedParams.PropertyPath.GetLeafMostProperty().Property.Get());
	if (!TextProperty)
	{
		return;
	}
#endif

	FGameplayTag GameplayTagPropertyValue = PropertyChangedParams.GetPropertyValue<FGameplayTag>();
	OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FMovieSceneGameplayTagChannel>(0, MoveTemp(GameplayTagPropertyValue), true));
}