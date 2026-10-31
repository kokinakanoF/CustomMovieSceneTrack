// Fill out your copyright notice in the Description page of Project Settings.

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

void UMovieSceneGameplayTagSection::ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity)
{
	using namespace UE::MovieScene;

	if (!GameplayTagChannel.HasAnyData())
	{
		return;
	}

	const FBuiltInComponentTypes* Components = FBuiltInComponentTypes::Get();
	const FMovieSceneTracksComponentTypes* TracksComponents = FMovieSceneTracksComponentTypes::Get();

#if 0
	FPropertyTrackEntityImportHelper(TracksComponents->Integer)
		.Add(Components->IntegerChannel, &IntegerCurve)
		.Add(Components->BlenderType, UMovieScenePiecewiseIntegerBlenderSystem::StaticClass())
		.Commit(this, Params, OutImportedEntity);
#endif
}

bool UMovieSceneGameplayTagSection::PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange, const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder)
{
	FMovieScenePropertyTrackEntityImportHelper::PopulateEvaluationField(*this, EffectiveRange, InMetaData, OutFieldBuilder);
	return true;
}
