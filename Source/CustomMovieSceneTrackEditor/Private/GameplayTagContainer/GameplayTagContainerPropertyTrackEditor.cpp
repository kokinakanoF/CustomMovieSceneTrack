// Copyright Koki Nakano. All Rights Reserved.

#include "GameplayTagContainer/GameplayTagContainerPropertyTrackEditor.h"

#include "GameplayTagContainer/MovieSceneGameplayTagContainerChannel.h"

bool CanCreateKeyEditor(const FMovieSceneGameplayTagContainerChannel* Channel)
{
	return true;
}

TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagContainerChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params)
{
	const TMovieSceneExternalValue<FGameplayTagContainer>* ExternalValue = Channel.GetExtendedEditorData();
	if (!ExternalValue)
	{
		return SNullWidget::NullWidget;
	}

	TSequencerKeyEditor<FMovieSceneGameplayTagContainerChannel, FGameplayTagContainer> KeyEditor(
		Params.ObjectBindingID, Channel,
		Params.OwningSection, Params.Sequencer, Params.PropertyBindings, ExternalValue->OnGetExternalValue
	);
	KeyEditor.SetOwningObject(Cast<UMovieSceneSignedObject>(Params.OwningObject));

	return SNew(SGameplayTagContainerKeyEditor, KeyEditor);
}




TSharedRef<ISequencerTrackEditor> FGameplayTagContainerPropertyTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
{
	return MakeShared<FGameplayTagContainerPropertyTrackEditor>(OwningSequencer);
}

void FGameplayTagContainerPropertyTrackEditor::GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams
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

	FGameplayTagContainer GameplayTagContainerPropertyValue = PropertyChangedParams.GetPropertyValue<FGameplayTagContainer>();
	OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FMovieSceneGameplayTagContainerChannel>(0, MoveTemp(GameplayTagContainerPropertyValue), true));
}