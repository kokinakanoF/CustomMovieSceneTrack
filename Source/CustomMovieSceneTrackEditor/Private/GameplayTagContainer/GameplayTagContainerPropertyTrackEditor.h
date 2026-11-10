// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "PropertyTrackEditor.h"
#include "GameplayTagContainer.h"
#include "Channels/MovieSceneChannelHandle.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerTrack.h"
#include "GameplayTagContainer/SGameplayTagContainerKeyEditor.h"

class UMovieSceneKeyStructType;
class FSequencerKeyStructGenerator;
struct FMovieSceneGameplayTagContainerChannel;

CUSTOMMOVIESCENETRACKEDITOR_API bool CanCreateKeyEditor(const FMovieSceneGameplayTagContainerChannel* Channel);
CUSTOMMOVIESCENETRACKEDITOR_API TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagContainerChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params);

/** Track Editor for GameplayTagContainer Property */
class FGameplayTagContainerPropertyTrackEditor : public FPropertyTrackEditor<UMovieSceneGameplayTagContainerTrack>
{
public:

	FGameplayTagContainerPropertyTrackEditor(TSharedRef<ISequencer> InSequencer)
		: FPropertyTrackEditor(InSequencer, GetAnimatedPropertyTypes())
	{
	}

	/** Retrieve a list of all property types that this track editor animates  */
	static TArray<FAnimatedPropertyKey, TInlineAllocator<1>> GetAnimatedPropertyTypes()
	{
		return TArray<FAnimatedPropertyKey, TInlineAllocator<1>>({ FAnimatedPropertyKey::FromStructType(FGameplayTagContainer::StaticStruct()) });
	}
	
	/**
	 * Creates an instance of this class (called by a sequencer).
	 * @param OwningSequencer	The sequencer instance to be used by this tool
	 * @return The new instance of this class
	 */
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);
	
protected:

	//~ Begin FPropertyTrackEditor
	virtual void GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams, UMovieSceneSection* SectionToKey, FGeneratedTrackKeys& OutGeneratedKeys) override;
	//~ End FPropertyTrackEditor
};