// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "MovieSceneSection.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerChannel.h"
#include "MovieSceneGameplayTagContainerSection.generated.h"

/**
 * Section class that enables GameplayTagContainers to be manipulated in Sequencer.
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagContainerSection
	: public UMovieSceneSection
{
	GENERATED_BODY()

public:

	UMovieSceneGameplayTagContainerSection(const FObjectInitializer& InObjectInitializer);

	const FMovieSceneGameplayTagContainerChannel& GetChannel() const { return GameplayTagContainerChannel; }

protected:

	UPROPERTY()
	FMovieSceneGameplayTagContainerChannel GameplayTagContainerChannel;
};
