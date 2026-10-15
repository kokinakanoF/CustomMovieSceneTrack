// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Evaluation/MovieScenePropertyTemplate.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"
#include "MovieSceneGameplayTagTemplate.generated.h"

class UMovieScenePropertyTrack;
class UMovieSceneGameplayTagSection;
struct FMovieSceneGameplayTagChannel;

USTRUCT()
struct FMovieSceneGameplayTagPropertySectionTemplate
	: public FMovieScenePropertySectionTemplate
{
	GENERATED_BODY()

public:
	FMovieSceneGameplayTagPropertySectionTemplate() {}
	FMovieSceneGameplayTagPropertySectionTemplate(const UMovieSceneGameplayTagSection& Section, const UMovieScenePropertyTrack& Track);

protected:
	//~Begin FMovieScenePropertySectionTemplate interface
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag); }
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	//~End FMovieScenePropertySectionTemplate interface

protected:
	UPROPERTY()
	FMovieSceneGameplayTagChannel GameplayTagChannel;
};
