// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTag/MovieSceneGameplayTagTrack.h"

#include "Evaluation/MovieSceneEvalTemplate.h"

#include "GameplayTag/MovieSceneGameplayTagSection.h"
#include "GameplayTag/MovieSceneGameplayTagTemplate.h"

UMovieSceneSection* UMovieSceneGameplayTagTrack::CreateNewSection()
{
	// Undo/RedoÇ…ëŒâûÇ∑ÇÈÇΩÇﬂÅARF_TransactionalÇéwíË
	return NewObject<UMovieSceneGameplayTagSection>(this, NAME_None, RF_Transactional);
}

bool UMovieSceneGameplayTagTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return (SectionClass == UMovieSceneGameplayTagSection::StaticClass());
}

FMovieSceneEvalTemplatePtr UMovieSceneGameplayTagTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneGameplayTagPropertySectionTemplate(*CastChecked<const UMovieSceneGameplayTagSection>(&InSection), *this);
}
