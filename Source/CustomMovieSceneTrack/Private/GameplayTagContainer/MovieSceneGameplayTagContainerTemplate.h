// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "Evaluation/MovieScenePropertyTemplate.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerChannel.h"
#include "MovieSceneGameplayTagContainerTemplate.generated.h"

class UMovieScenePropertyTrack;
class UMovieSceneGameplayTagContainerSection;

USTRUCT()
struct FMovieSceneGameplayTagContainerPropertySectionTemplate
	: public FMovieScenePropertySectionTemplate
{
	GENERATED_BODY()

public:
	FMovieSceneGameplayTagContainerPropertySectionTemplate() {}
	FMovieSceneGameplayTagContainerPropertySectionTemplate(const UMovieSceneGameplayTagContainerSection& Section, const UMovieScenePropertyTrack& Track);

protected:
	//~Begin FMovieScenePropertySectionTemplate interface
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag); }
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	//~End FMovieScenePropertySectionTemplate interface

protected:
	UPROPERTY()
	FMovieSceneGameplayTagContainerChannel GameplayTagContainerChannel;
};
