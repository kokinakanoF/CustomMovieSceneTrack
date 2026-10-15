// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTag/MovieSceneGameplayTagChannel.h"

bool FMovieSceneGameplayTagChannel::Evaluate(FFrameTime InTime, FGameplayTag& OutValue) const
{
	if (const FGameplayTag* Result = Evaluate(InTime))
	{
		OutValue = *Result;
		return true;
	}

	return false;
}

const FGameplayTag* FMovieSceneGameplayTagChannel::Evaluate(FFrameTime InTime) const
{
	if (Times.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(Times, InTime.FrameNumber) - 1);
		return &Values[Index];
		 true;
	}

	return bHasDefaultValue ? &DefaultValue : nullptr;
}

void FMovieSceneGameplayTagChannel::GetKeys(const TRange<FFrameNumber>& WithinRange
	, TArray<FFrameNumber>* OutKeyTimes
	, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FMovieSceneGameplayTagChannel::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FMovieSceneGameplayTagChannel::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FMovieSceneGameplayTagChannel::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FMovieSceneGameplayTagChannel::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FMovieSceneGameplayTagChannel::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	// Insert a key at the current time to maintain evaluation
	if (GetData().GetTimes().Num() > 0)
	{
		if (const FGameplayTag* CurrentValue = Evaluate(InTime))
		{
			FGameplayTag Value(*CurrentValue);
			GetData().UpdateOrAddKey(InTime, Value);
		}
	}
	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FMovieSceneGameplayTagChannel::RemapTimes(const UE::MovieScene::IRetimingInterface& Retimer)
{
	GetData().RemapTimes(Retimer);
}

TRange<FFrameNumber> FMovieSceneGameplayTagChannel::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FMovieSceneGameplayTagChannel::GetNumKeys() const
{
	return Times.Num();
}

void FMovieSceneGameplayTagChannel::Reset()
{
	Times.Reset();
	Values.Reset();
	KeyHandles.Reset();
	bHasDefaultValue = false;
}

void FMovieSceneGameplayTagChannel::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

void FMovieSceneGameplayTagChannel::Optimize(const FKeyDataOptimizationParams& InParameters)
{
	UE::MovieScene::Optimize(this, InParameters);
}

void FMovieSceneGameplayTagChannel::ClearDefault()
{
	bHasDefaultValue = false;
}
