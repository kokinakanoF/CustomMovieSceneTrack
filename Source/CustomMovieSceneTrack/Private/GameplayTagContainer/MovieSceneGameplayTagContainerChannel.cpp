// Copyright Koki Nakano. All Rights Reserved.

#include "GameplayTagContainer/MovieSceneGameplayTagContainerChannel.h"

bool FMovieSceneGameplayTagContainerChannel::Evaluate(FFrameTime InTime, FGameplayTagContainer& OutValue) const
{
	if (const FGameplayTagContainer* Result = Evaluate(InTime))
	{
		OutValue = *Result;
		return true;
	}

	return false;
}

const FGameplayTagContainer* FMovieSceneGameplayTagContainerChannel::Evaluate(FFrameTime InTime) const
{
	if (Times.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(Times, InTime.FrameNumber) - 1);
		return &Values[Index];
		 true;
	}

	return bHasDefaultValue ? &DefaultValue : nullptr;
}

void FMovieSceneGameplayTagContainerChannel::GetKeys(const TRange<FFrameNumber>& WithinRange
	, TArray<FFrameNumber>* OutKeyTimes
	, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FMovieSceneGameplayTagContainerChannel::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FMovieSceneGameplayTagContainerChannel::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FMovieSceneGameplayTagContainerChannel::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FMovieSceneGameplayTagContainerChannel::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FMovieSceneGameplayTagContainerChannel::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	// Insert a key at the current time to maintain evaluation
	if (GetData().GetTimes().Num() > 0)
	{
		if (const FGameplayTagContainer* CurrentValue = Evaluate(InTime))
		{
			FGameplayTagContainer Value(*CurrentValue);
			GetData().UpdateOrAddKey(InTime, Value);
		}
	}
	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FMovieSceneGameplayTagContainerChannel::RemapTimes(const UE::MovieScene::IRetimingInterface& Retimer)
{
	GetData().RemapTimes(Retimer);
}

TRange<FFrameNumber> FMovieSceneGameplayTagContainerChannel::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FMovieSceneGameplayTagContainerChannel::GetNumKeys() const
{
	return Times.Num();
}

void FMovieSceneGameplayTagContainerChannel::Reset()
{
	Times.Reset();
	Values.Reset();
	KeyHandles.Reset();
	bHasDefaultValue = false;
}

void FMovieSceneGameplayTagContainerChannel::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

void FMovieSceneGameplayTagContainerChannel::Optimize(const FKeyDataOptimizationParams& InParameters)
{
	UE::MovieScene::Optimize(this, InParameters);
}

void FMovieSceneGameplayTagContainerChannel::ClearDefault()
{
	bHasDefaultValue = false;
}
