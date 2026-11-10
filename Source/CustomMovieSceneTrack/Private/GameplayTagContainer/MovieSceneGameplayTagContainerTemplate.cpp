// Copyright Koki Nakano. All Rights Reserved.

#include "GameplayTagContainer/MovieSceneGameplayTagContainerTemplate.h"

#include "Tracks/MovieScenePropertyTrack.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerSection.h"

FMovieSceneGameplayTagContainerPropertySectionTemplate::FMovieSceneGameplayTagContainerPropertySectionTemplate(const UMovieSceneGameplayTagContainerSection& Section, const UMovieScenePropertyTrack& Track)
	: FMovieScenePropertySectionTemplate(Track.GetPropertyName(), Track.GetPropertyPath().ToString())
	, GameplayTagContainerChannel(Section.GetChannel())
{
}

void FMovieSceneGameplayTagContainerPropertySectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Only evaluate if the curve has any data
	FGameplayTagContainer Result;
	if (GameplayTagContainerChannel.Evaluate(Context.GetTime(), Result))
	{
		ExecutionTokens.Add(TPropertyTrackExecutionToken<FGameplayTagContainer>(Result));
	}
}
