// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "GameplayTagContainer.h"
#include "MovieSceneGameplayTagChannel.generated.h"

/**
 * Structure that manages key, value, and other information for GameplayTag sections.
 */
USTRUCT()
struct FMovieSceneGameplayTagChannel : public FMovieSceneChannel
{
	GENERATED_BODY()

	/**
	 * Define the key type as CurveValueType so it can be handled by shared template logic.
	 * This name is used by DeleteKeys() in SequencerChannelTraits.h.
	 */
	typedef FGameplayTag CurveValueType;

public:

	/**
	 * Access a mutable interface for this channel's data
	 * @return An object that is able to manipulate this channel's data
	 */
	inline TMovieSceneChannelData<FGameplayTag> GetData()
	{
		return TMovieSceneChannelData<FGameplayTag>(&Times, &Values, this, &KeyHandles);
	}

	/**
	 * Access a constant interface for this channel's data
	 * @return An object that is able to interrogate this channel's data
	 */
	inline TMovieSceneChannelData<const FGameplayTag> GetData() const
	{
		return TMovieSceneChannelData<const FGameplayTag>(&Times, &Values);
	}

	inline TConstArrayView<FFrameNumber> GetTimes() const
	{
		return Times;
	}

	/** Returns whether this channel has any values */
	inline bool HasAnyData() const
	{
		return Times.Num() != 0 || bHasDefaultValue == true;
	}

	/**
	 * Evaluate this channel
	 *
	 * @param InTime	The time to evaluate at
	 * @param OutValue	A value to receive the result
	 * @return true if the channel was evaluated successfully, false otherwise
	 */
	CUSTOMMOVIESCENETRACK_API bool Evaluate(FFrameTime InTime, FGameplayTag& OutValue) const;
	const FGameplayTag* Evaluate(FFrameTime InTime) const;

	/**
	 * Add keys with these times to channel. The number of elements in both arrays much match or nothing is added.
	 * @param InTimes	Times to add
	 * @param InValues	Values to add
	 */
	inline void AddKeys(const TArray<FFrameNumber>& InTimes, const TArray<FGameplayTag>& InValues)
	{
		check(InTimes.Num() == InValues.Num());
		Times.Append(InTimes);
		Values.Append(InValues);
	}

	/**
	 * Set this channel's default value that should be used when no keys are present
	 * @param InDefaultValue	The desired default value
	 */
	inline void SetDefault(FGameplayTag InDefaultValue)
	{
		bHasDefaultValue = true;
		DefaultValue = InDefaultValue;
	}

	/**
	 * Get this channel's default value that will be used when no keys are present
	 * @return (Optional) The channel's default value
	 */
	inline TOptional<FGameplayTag> GetDefault() const
	{
		return bHasDefaultValue ? TOptional<FGameplayTag>(DefaultValue) : TOptional<FGameplayTag>();
	}

	/** Remove this channel's default value causing the channel to have no effect where no keys are present */
	inline void RemoveDefault()
	{
		bHasDefaultValue = false;
	}

	//~ Begin FMovieSceneChannel
	CUSTOMMOVIESCENETRACK_API virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	CUSTOMMOVIESCENETRACK_API virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	CUSTOMMOVIESCENETRACK_API virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	CUSTOMMOVIESCENETRACK_API virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	CUSTOMMOVIESCENETRACK_API virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	CUSTOMMOVIESCENETRACK_API virtual void DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore) override;
	CUSTOMMOVIESCENETRACK_API virtual void RemapTimes(const UE::MovieScene::IRetimingInterface& Retimer) override;
	CUSTOMMOVIESCENETRACK_API virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	CUSTOMMOVIESCENETRACK_API virtual int32 GetNumKeys() const override;
	CUSTOMMOVIESCENETRACK_API virtual void Reset() override;
	CUSTOMMOVIESCENETRACK_API virtual void Offset(FFrameNumber DeltaPosition) override;
	CUSTOMMOVIESCENETRACK_API virtual void Optimize(const FKeyDataOptimizationParams& InParameters) override;
	CUSTOMMOVIESCENETRACK_API virtual void ClearDefault() override;
	//~ End FMovieSceneChannel

private:

	UPROPERTY(meta = (KeyTimes))
	TArray<FFrameNumber> Times;

	/** Array of values that correspond to each key time */
	UPROPERTY(meta = (KeyValues))
	TArray<FGameplayTag> Values;

	/** Default value used when there are no keys */
	UPROPERTY()
	FGameplayTag DefaultValue;

	UPROPERTY()
	bool bHasDefaultValue = false;

	/** This needs to be a UPROPERTY so it gets saved into editor transactions but transient so it does not get saved into assets. */
	UPROPERTY(Transient)
	FMovieSceneKeyHandleMap KeyHandles;
};

/**
 * Defines the channel traits for FMovieSceneGameplayTagChannel.
 *
 * Provides the extended editor data type used to retrieve FGameplayTag
 * values from a TMovieSceneChannelHandle<FMovieSceneGameplayTagChannel>.
 */
template<>
struct TMovieSceneChannelTraits<FMovieSceneGameplayTagChannel> : TMovieSceneChannelTraitsBase<FMovieSceneGameplayTagChannel>
{
#if WITH_EDITOR

	/** Integer channels can have external values (ie, they can get their values from external objects for UI purposes) */
	typedef TMovieSceneExternalValue<FGameplayTag> ExtendedEditorDataType;

#endif
};
