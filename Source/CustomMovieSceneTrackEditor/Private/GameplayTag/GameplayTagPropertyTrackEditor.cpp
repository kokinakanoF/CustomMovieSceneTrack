// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "ISequencerChannelInterface.h"
#include "SequencerKeyStructGenerator.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"

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
