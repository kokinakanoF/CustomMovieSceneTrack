// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MovieSceneGameplayTagTrack.generated.h"

/**
 * Custom track class that enables GameplayTags to be manipulated in Sequencer.
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagTrack
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
