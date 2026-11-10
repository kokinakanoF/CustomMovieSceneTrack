// Copyright Koki Nakano. All Rights Reserved.

#include "GameplayTagContainer/MovieSceneGameplayTagContainerTrack.h"

#include "Evaluation/MovieSceneEvalTemplate.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerSection.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerTemplate.h"

UMovieSceneSection* UMovieSceneGameplayTagContainerTrack::CreateNewSection()
{
	// Set RF_Transactional to support Undo/Redo.
	return NewObject<UMovieSceneGameplayTagContainerSection>(this, NAME_None, RF_Transactional);
}

bool UMovieSceneGameplayTagContainerTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return (SectionClass == UMovieSceneGameplayTagContainerSection::StaticClass());
}

FMovieSceneEvalTemplatePtr UMovieSceneGameplayTagContainerTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneGameplayTagContainerPropertySectionTemplate(*CastChecked<const UMovieSceneGameplayTagContainerSection>(&InSection), *this);
}
