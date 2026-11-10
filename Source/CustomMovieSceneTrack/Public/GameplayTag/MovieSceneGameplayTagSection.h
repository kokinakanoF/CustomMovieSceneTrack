// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"
#include "MovieSceneGameplayTagSection.generated.h"

/**
 * Section class that enables GameplayTags to be manipulated in Sequencer.
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagSection
	: public UMovieSceneSection
{
	GENERATED_BODY()

public:

	UMovieSceneGameplayTagSection(const FObjectInitializer& InObjectInitializer);
	 
	const FMovieSceneGameplayTagChannel& GetChannel() const { return GameplayTagChannel; }

protected:

	UPROPERTY()
	FMovieSceneGameplayTagChannel GameplayTagChannel;
};
