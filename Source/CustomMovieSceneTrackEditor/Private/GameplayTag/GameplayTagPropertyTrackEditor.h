// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PropertyTrackEditor.h"
#include "GameplayTagContainer.h"
#include "Channels/MovieSceneChannelHandle.h"
#include "GameplayTag/MovieSceneGameplayTagTrack.h"
#include "GameplayTag/SGameplayTagCurveKeyEditor.h"


class UMovieSceneKeyStructType;
class FSequencerKeyStructGenerator;
struct FMovieSceneGameplayTagChannel;


CUSTOMMOVIESCENETRACKEDITOR_API bool CanCreateKeyEditor(const FMovieSceneGameplayTagChannel* Channel);
CUSTOMMOVIESCENETRACKEDITOR_API TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params);


/** Track Editor for GameplayTag Property */
class FGameplayTagPropertyTrackEditor : public FPropertyTrackEditor<UMovieSceneGameplayTagTrack>
{
public:
	FGameplayTagPropertyTrackEditor(TSharedRef<ISequencer> InSequencer)
		: FPropertyTrackEditor(InSequencer, GetAnimatedPropertyTypes())
	{
	}

	/** Retrieve a list of all property types that this track editor animates  */
	static TArray<FAnimatedPropertyKey, TInlineAllocator<1>> GetAnimatedPropertyTypes()
	{
		return TArray<FAnimatedPropertyKey, TInlineAllocator<1>>({ FAnimatedPropertyKey::FromStructType(FGameplayTag::StaticStruct()) });
	}
	
	/**
	 * Creates an instance of this class (called by a sequencer).
	 * @param OwningSequencer The sequencer instance to be used by this tool
	 * @return The new instance of this class
	 */
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);
	
protected:
	//~ Begin FPropertyTrackEditor
	virtual void GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams, UMovieSceneSection* SectionToKey, FGeneratedTrackKeys& OutGeneratedKeys) override;
	//~ End FPropertyTrackEditor
};