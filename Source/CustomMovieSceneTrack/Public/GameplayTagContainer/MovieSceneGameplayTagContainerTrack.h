// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "Tracks/MovieScenePropertyTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MovieSceneGameplayTagContainerTrack.generated.h"

/**
 * Custom track class that enables GameplayTagContainers to be manipulated in Sequencer.
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagContainerTrack
	: public UMovieScenePropertyTrack
	, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()
	
public:

	//~Begin UMovieSceneTrack interface
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	//~End UMovieSceneTrack interface

	//~Begin IMovieSceneTrackTemplateProducer interface
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
	//~End IMovieSceneTrackTemplateProducer interface
};
