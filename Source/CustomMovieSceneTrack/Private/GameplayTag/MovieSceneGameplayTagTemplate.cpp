// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTag/MovieSceneGameplayTagTemplate.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "GameplayTag/MovieSceneGameplayTagSection.h"

FMovieSceneGameplayTagPropertySectionTemplate::FMovieSceneGameplayTagPropertySectionTemplate(const UMovieSceneGameplayTagSection& Section, const UMovieScenePropertyTrack& Track)
	: FMovieScenePropertySectionTemplate(Track.GetPropertyName(), Track.GetPropertyPath().ToString())
	, GameplayTagChannel(Section.GetChannel())
{
}

void FMovieSceneGameplayTagPropertySectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Only evaluate if the curve has any data
	FGameplayTag Result;
	if (GameplayTagChannel.Evaluate(Context.GetTime(), Result))
	{
		ExecutionTokens.Add(TPropertyTrackExecutionToken<FGameplayTag>(Result));
	}
}
