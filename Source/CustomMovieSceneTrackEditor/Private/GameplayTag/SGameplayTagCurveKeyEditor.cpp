// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGameplayTagCurveKeyEditor.h"
// GameplayTagsEditor
#include "SGameplayTagCombo.h"
// MovieScene
#include "TrackInstancePropertyBindings.h"
// PropertyEditor
#include "IDetailKeyframeHandler.h"
// Sequencer
#include "ISequencer.h"
#include "ISequencerPropertyKeyedStatus.h"
#include "MovieSceneTrackEditor.h"
// Slate
#include "Widgets/Layout/SBox.h"
// UnrealEd
#include "ScopedTransaction.h"

// CustomMovieSceneTrack
#include "GameplayTag/MovieSceneGameplayTagTrack.h"
#include "GameplayTag/MovieSceneGameplayTagSection.h"

#define LOCTEXT_NAMESPACE "SGameplayTagCurveKeyEditor"


SGameplayTagCurveKeyEditor::~SGameplayTagCurveKeyEditor()
{
	// Unbind the callback.
	if (GameplayTagProperty)
	{
		ISequencerPropertyKeyedStatusHandler& PropertyKeyedStatusHandler = KeyEditor.GetSequencer()->GetPropertyKeyedStatusHandler();
		PropertyKeyedStatusHandler.GetExternalHandler(GameplayTagProperty).Unbind();

		GameplayTagProperty = nullptr;
	}
}

void SGameplayTagCurveKeyEditor::Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag>& InKeyEditor)
{
	KeyEditor = InKeyEditor;

	// Create a widget that allows a GameplayTag to be set.
	// Because the button portion of SGameplayTagCombo is not included in DesiredSize,
	// using it directly would cause it to extend beyond the panel.
	// Use a HorizontalBox for clipping to prevent this.
	ChildSlot
	[
		SNew(SHorizontalBox)
			.Clipping(EWidgetClipping::OnDemand)
			+ SHorizontalBox::Slot()
			[
				SNew(SGameplayTagCombo)
					.Tag(this, &SGameplayTagCurveKeyEditor::GetGameplayTag)
					.OnTagChanged(this, &SGameplayTagCurveKeyEditor::OnTagChanged)
			]
	];

	// Bind callbacks to retrieve key states using a custom algorithm.
	// Bind them for each class member variable.
	TArrayView<TWeakObjectPtr<>> Objects = InKeyEditor.GetSequencer()->FindObjectsInCurrentSequence(InKeyEditor.GetObjectBindingID());
	for (TWeakObjectPtr<> Object : Objects)
	{
		GameplayTagProperty = InKeyEditor.GetPropertyBindings()->GetProperty(*Object.Get());
		if (GameplayTagProperty)
		{
			ISequencerPropertyKeyedStatusHandler& PropertyKeyedStatusHandler = InKeyEditor.GetSequencer()->GetPropertyKeyedStatusHandler();
			PropertyKeyedStatusHandler.GetExternalHandler(GameplayTagProperty).BindRaw(this, &SGameplayTagCurveKeyEditor::GetPropertyKeyedStatus);
		}
	}
}

FGameplayTag SGameplayTagCurveKeyEditor::GetGameplayTag() const
{
	return KeyEditor.GetCurrentValue();
}

void SGameplayTagCurveKeyEditor::OnTagChanged(const FGameplayTag InTag)
{
	FScopedTransaction Transaction(LOCTEXT("SetGameplayTagKey", "Set GameplayTag Key Value"));
	KeyEditor.SetValueWithNotify(InTag, EMovieSceneDataChangeType::TrackValueChangedRefreshImmediately);
}

EPropertyKeyedStatus SGameplayTagCurveKeyEditor::GetPropertyKeyedStatus(const IPropertyHandle& InPropertyHandle) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FGameplayTagPropertyTrackEditor::GetPropertyKeyedStatus);

	ISequencer* SequencerPtr = KeyEditor.GetSequencer();
	if (!SequencerPtr)
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	UMovieSceneSequence* Sequence = SequencerPtr->GetFocusedMovieSceneSequence();
	UMovieScene* MovieScene = Sequence ? Sequence->GetMovieScene() : nullptr;
	if (!MovieScene)
	{
		return EPropertyKeyedStatus::NotKeyed;
	}

	// Get the property owner.
	TArray<UObject*> OuterObjects;
	InPropertyHandle.GetOuterObjects(OuterObjects);
	if (OuterObjects.IsEmpty())
	{
		return EPropertyKeyedStatus::NotKeyed;
	}


	const TRange<FFrameNumber> FrameRange = TRange<FFrameNumber>(SequencerPtr->GetLocalTime().Time.FrameNumber);
	EPropertyKeyedStatus KeyedStatus = EPropertyKeyedStatus::NotKeyed;

	for (UObject* Object : OuterObjects)
	{
		// Get the track.
		constexpr bool bCreateHandleIfMissing = false;
		FGuid OwingObjectHandle = SequencerPtr->GetHandleToObject(Object, bCreateHandleIfMissing);
		UMovieSceneGameplayTagTrack* GameplayTagTrack = MovieScene->FindTrack<UMovieSceneGameplayTagTrack>(OwingObjectHandle, InPropertyHandle.GetProperty()->GetFName());
		if (!GameplayTagTrack || GameplayTagTrack->IsEmpty())
		{
			continue;
		}
	
		// Check the key state in each section.
		for (const UMovieSceneSection* Section : GameplayTagTrack->GetAllSections())
		{
			if (const UMovieSceneGameplayTagSection* GameplayTagSection = Cast<UMovieSceneGameplayTagSection>(Section))
			{
				EPropertyKeyedStatus NewKeyedStatus = GetKeyedStatusInSection(*GameplayTagSection, FrameRange);
				KeyedStatus = FMath::Max(KeyedStatus, NewKeyedStatus);

				// Stop checking once the key is confirmed to exist.
				if (KeyedStatus == EPropertyKeyedStatus::KeyedInFrame)
				{
					return KeyedStatus;
				}
			}
		}
	}

	return KeyedStatus;
}

EPropertyKeyedStatus SGameplayTagCurveKeyEditor::GetKeyedStatusInSection(const UMovieSceneGameplayTagSection& InSection, const TRange<FFrameNumber>& InRange) const
{
	EPropertyKeyedStatus SectionKeyedStatus = EPropertyKeyedStatus::NotKeyed;
	int32 EmptyChannelCount = 0;

	TArrayView<FMovieSceneGameplayTagChannel*> Channels = InSection.GetChannelProxy().GetChannels<FMovieSceneGameplayTagChannel>();
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

		// At this point, the key is confirmed to exist.
		SectionKeyedStatus = FMath::Max(SectionKeyedStatus, EPropertyKeyedStatus::KeyedInOtherFrame);

		TArray<FFrameNumber> KeyTimes;
		Channel->GetKeys(InRange, &KeyTimes, nullptr);
		if (KeyTimes.IsEmpty())
		{
			++EmptyChannelCount;
		}
		else
		{
			// At this point, the key is confirmed to exist on the frame.
			SectionKeyedStatus = FMath::Max(SectionKeyedStatus, EPropertyKeyedStatus::KeyedInFrame);
		}
	}

	if (EmptyChannelCount == 0 && SectionKeyedStatus == EPropertyKeyedStatus::PartiallyKeyed)
	{
		SectionKeyedStatus = EPropertyKeyedStatus::KeyedInFrame;
	}

	return SectionKeyedStatus;
}

#undef LOCTEXT_NAMESPACE
