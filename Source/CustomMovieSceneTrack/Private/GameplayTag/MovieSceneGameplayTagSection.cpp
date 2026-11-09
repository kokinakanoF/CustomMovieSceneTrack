// Copyright Koki Nakano.All Rights Reserved.

#include "GameplayTag/MovieSceneGameplayTagSection.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "GameplayTagContainer.h"

UMovieSceneGameplayTagSection::UMovieSceneGameplayTagSection(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	bSupportsInfiniteRange = true;
	EvalOptions.EnableAndSetCompletionMode(EMovieSceneCompletionMode::ProjectDefault);

#if WITH_EDITOR
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(GameplayTagChannel, FMovieSceneChannelMetaData(), TMovieSceneExternalValue<FGameplayTag>::Make());
#else
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(GameplayTagChannel);
#endif
}