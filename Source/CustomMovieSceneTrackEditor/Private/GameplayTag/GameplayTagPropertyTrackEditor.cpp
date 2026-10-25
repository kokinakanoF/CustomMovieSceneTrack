// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTag/GameplayTagPropertyTrackEditor.h"
#include "MovieSceneTrackEditor.h"
#include "MovieScene.h"
#include "ISequencerPropertyKeyedStatus.h"
#include "ISequencer.h"
#include "ISequencerChannelInterface.h"
#include "IDetailKeyframeHandler.h"
#include "SequencerKeyStructGenerator.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"
#include "GameplayTag/MovieSceneGameplayTagSection.h"

bool CanCreateKeyEditor(const FMovieSceneGameplayTagChannel* Channel)
{
	return true;
}

TSharedRef<SWidget> CreateKeyEditor(const TMovieSceneChannelHandle<FMovieSceneGameplayTagChannel>& Channel, const UE::Sequencer::FCreateKeyEditorParams& Params)
{
	const TMovieSceneExternalValue<FGameplayTag>* ExternalValue = Channel.GetExtendedEditorData();
	if (!ExternalValue)
	{
		return SNullWidget::NullWidget;
	}

	TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag> KeyEditor(
		Params.ObjectBindingID, Channel,
		Params.OwningSection, Params.Sequencer, Params.PropertyBindings, ExternalValue->OnGetExternalValue
	);
	KeyEditor.SetOwningObject(Cast<UMovieSceneSignedObject>(Params.OwningObject));

	return SNew(SGameplayTagCurveKeyEditor, KeyEditor);
}




TSharedRef<ISequencerTrackEditor> FGameplayTagPropertyTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
{
	return MakeShared<FGameplayTagPropertyTrackEditor>(OwningSequencer);
}

void FGameplayTagPropertyTrackEditor::GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams
	, UMovieSceneSection* SectionToKey
	, FGeneratedTrackKeys& OutGeneratedKeys)
{
	// TODO:この処理が必要か確認
#if 0
	const FTextProperty* TextProperty = CastField<const FTextProperty>(PropertyChangedParams.PropertyPath.GetLeafMostProperty().Property.Get());
	if (!TextProperty)
	{
		return;
	}
#endif

	FGameplayTag GameplayTagPropertyValue = PropertyChangedParams.GetPropertyValue<FGameplayTag>();
	OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FMovieSceneGameplayTagChannel>(0, MoveTemp(GameplayTagPropertyValue), true));
}

void FGameplayTagPropertyTrackEditor::InitializeNewTrack(UMovieSceneGameplayTagTrack* NewTrack, FPropertyChangedParams PropertyChangedParams)
{
	FPropertyTrackEditor<UMovieSceneGameplayTagTrack>::InitializeNewTrack(NewTrack, PropertyChangedParams);

	if (TSharedPtr<ISequencer> SequencerPtr = FMovieSceneTrackEditor::GetSequencer())
	{
		const FProperty* GameplayTagProperty = PropertyChangedParams.PropertyPath.GetLeafMostProperty().Property.Get();
		if (GameplayTagProperty)
		{
			ISequencerPropertyKeyedStatusHandler& PropertyKeyedStatusHandler = SequencerPtr->GetPropertyKeyedStatusHandler();
			PropertyKeyedStatusHandler.GetExternalHandler(GameplayTagProperty)
				.BindRaw(this, &FGameplayTagPropertyTrackEditor::GetPropertyKeyedStatus);
		}
	}
}

EPropertyKeyedStatus FGameplayTagPropertyTrackEditor::GetKeyedStatusInSection(const UMovieSceneGameplayTagSection& Section, const TRange<FFrameNumber>& Range) const
{
	EPropertyKeyedStatus SectionKeyedStatus = EPropertyKeyedStatus::NotKeyed;

	int32 EmptyChannelCount = 0;

	TArrayView<FMovieSceneGameplayTagChannel*> Channels = Section.GetChannelProxy().GetChannels<FMovieSceneGameplayTagChannel>();
	for (FMovieSceneGameplayTagChannel* Channel : Channels)
	{
		if (!Channel)
		{
			continue;
		}

		if (Channel->GetNumKeys() == 0)
		{
			++EmptyChannelCount;
			continue;
		}

		SectionKeyedStatus = FMath::Max(SectionKeyedStatus, EPropertyKeyedStatus::KeyedInOtherFrame);

		TArray<FFrameNumber> KeyTimes;
		Channel->GetKeys(Range, &KeyTimes, nullptr);
		if (KeyTimes.IsEmpty())
		{
			++EmptyChannelCount;
		}
		else
		{
			SectionKeyedStatus = FMath::Max(SectionKeyedStatus, EPropertyKeyedStatus::PartiallyKeyed);
		}
	}

	if (EmptyChannelCount == 0 && SectionKeyedStatus == EPropertyKeyedStatus::PartiallyKeyed)
	{
		SectionKeyedStatus = EPropertyKeyedStatus::KeyedInFrame;
	}
	return SectionKeyedStatus;
}

EPropertyKeyedStatus FGameplayTagPropertyTrackEditor::GetPropertyKeyedStatus(const IPropertyHandle& PropertyHandle) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FGameplayTagPropertyTrackEditor::GetPropertyKeyedStatus);

	TSharedPtr<ISequencer> SequencerPtr = FMovieSceneTrackEditor::GetSequencer();
	if (!SequencerPtr.IsValid())
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	UMovieSceneSequence* Sequence = SequencerPtr->GetFocusedMovieSceneSequence();

	UMovieScene* MovieScene = Sequence ? Sequence->GetMovieScene() : nullptr;
	if (!MovieScene)
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	TArray<UObject*> OuterObjects;
	PropertyHandle.GetOuterObjects(OuterObjects);
	if (OuterObjects.IsEmpty())
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	const TRange<FFrameNumber> FrameRange = TRange<FFrameNumber>(SequencerPtr->GetLocalTime().Time.FrameNumber);

	EPropertyKeyedStatus KeyedStatus = EPropertyKeyedStatus::NotKeyed;


	// トラックの取得
	constexpr bool bCreateHandleIfMissing = false;
	UMovieSceneGameplayTagTrack* GameplayTagTrack = nullptr;
	FGuid OwningActorHandle = SequencerPtr->GetHandleToObject(OuterObjects[0], bCreateHandleIfMissing);
	if (OwningActorHandle.IsValid())
	{
		GameplayTagTrack = MovieScene->FindTrack<UMovieSceneGameplayTagTrack>(OwningActorHandle, PropertyHandle.GetProperty()->GetFName());
	}

	if (!GameplayTagTrack || GameplayTagTrack->IsEmpty())
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	for (const UMovieSceneSection* Section : GameplayTagTrack->GetAllSections())
	{
		const UMovieSceneGameplayTagSection* GameplayTagSection = Cast<UMovieSceneGameplayTagSection>(Section);

		// Skip sections that were already processed
		if (GameplayTagSection)
		{
			EPropertyKeyedStatus NewKeyedStatus = GetKeyedStatusInSection(*GameplayTagSection, FrameRange);
			KeyedStatus = FMath::Max(KeyedStatus, NewKeyedStatus);

			// Maximum Status Reached no need to iterate further
			if (KeyedStatus >= EPropertyKeyedStatus::KeyedInOtherFrame)
			{
				return KeyedStatus;
			}
		}
	}

	return KeyedStatus;
}