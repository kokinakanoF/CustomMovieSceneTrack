// Copyright Koki Nakano. All Rights Reserved.

#include "GameplayTagContainer/MovieSceneGameplayTagContainerSection.h"

#include "Tracks/MovieScenePropertyTrack.h"
#include "GameplayTagContainer.h"

UMovieSceneGameplayTagContainerSection::UMovieSceneGameplayTagContainerSection(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	bSupportsInfiniteRange = true;
	EvalOptions.EnableAndSetCompletionMode(EMovieSceneCompletionMode::ProjectDefault);

#if WITH_EDITOR
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(GameplayTagContainerChannel, FMovieSceneChannelMetaData(), TMovieSceneExternalValue<FGameplayTagContainer>::Make());
#else
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(GameplayTagContainerChannel);
#endif
}