// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "EntitySystem/IMovieSceneEntityProvider.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"
#include "MovieSceneGameplayTagSection.generated.h"

/**
 * Section class that enables GameplayTags to be manipulated in Sequencer.
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagSection
	: public UMovieSceneSection
	, public IMovieSceneEntityProvider
{
	GENERATED_BODY()

public:

	UMovieSceneGameplayTagSection(const FObjectInitializer& InObjectInitializer);

	const FMovieSceneGameplayTagChannel& GetChannel() const { return GameplayTagChannel; }

	//~Begin IMovieSceneEntityProvider interface
	virtual void ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity) override;
	virtual bool PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange, const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder) override;
	//~End IMovieSceneEntityProvider interface
	
protected:

	UPROPERTY()
	FMovieSceneGameplayTagChannel GameplayTagChannel;
};
