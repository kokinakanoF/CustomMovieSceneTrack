// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTag/MovieSceneGameplayTagTrack.h"
#include "GameplayTag/MovieSceneGameplayTagSection.h"


UMovieSceneGameplayTagTrack::UMovieSceneGameplayTagTrack(const FObjectInitializer& ObjInit)
{
	SupportedBlendTypes.Add(EMovieSceneBlendType::Absolute);
	SupportedBlendTypes.Add(EMovieSceneBlendType::Additive);
}

UMovieSceneSection* UMovieSceneGameplayTagTrack::CreateNewSection()
{
	// Undo/RedoÇ…ëŒâûÇ∑ÇÈÇΩÇﬂÅARF_TransactionalÇéwíË
	return NewObject<UMovieSceneGameplayTagSection>(this, NAME_None, RF_Transactional);
}

bool UMovieSceneGameplayTagTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return (SectionClass == UMovieSceneGameplayTagSection::StaticClass());
}