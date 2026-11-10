// Copyright Koki Nakano. All Rights Reserved.

#include "SGameplayTagContainerKeyEditor.h"

#include "SGameplayTagContainerCombo.h"
#include "TrackInstancePropertyBindings.h"
#include "IDetailKeyframeHandler.h"
#include "ISequencer.h"
#include "ISequencerPropertyKeyedStatus.h"
#include "MovieSceneTrackEditor.h"
#include "Widgets/SBoxPanel.h"
#include "ScopedTransaction.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerTrack.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerSection.h"

#define LOCTEXT_NAMESPACE "SGameplayTagContainerKeyEditor"

SGameplayTagContainerKeyEditor::~SGameplayTagContainerKeyEditor()
{
	// Unbind the callback.
	if (GameplayTagContainerProperty)
	{
		ISequencerPropertyKeyedStatusHandler& PropertyKeyedStatusHandler = KeyEditor.GetSequencer()->GetPropertyKeyedStatusHandler();
		PropertyKeyedStatusHandler.GetExternalHandler(GameplayTagContainerProperty).Unbind();

		GameplayTagContainerProperty = nullptr;
	}
}

void SGameplayTagContainerKeyEditor::Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagContainerChannel, FGameplayTagContainer>& InKeyEditor)
{
	KeyEditor = InKeyEditor;

	// Create a widget that allows a GameplayTagContainer to be set.
	// Because the button portion of SGameplayTagContainerCombo is not included in DesiredSize,
	// using it directly would cause it to extend beyond the panel.
	// Use a HorizontalBox for clipping to prevent this.
	ChildSlot
	[
		SNew(SHorizontalBox)
			.Clipping(EWidgetClipping::OnDemand)
			+ SHorizontalBox::Slot()
			[
				SNew(SGameplayTagContainerCombo)
					.TagContainer(this, &SGameplayTagContainerKeyEditor::GetGameplayTagContainer)
					.OnTagContainerChanged(this, &SGameplayTagContainerKeyEditor::OnTagContainerChanged)
			]
	];

	// Bind callbacks to retrieve key states using a custom algorithm.
	// Bind them for each class member variable.
	TArrayView<TWeakObjectPtr<>> Objects = InKeyEditor.GetSequencer()->FindObjectsInCurrentSequence(InKeyEditor.GetObjectBindingID());
	for (TWeakObjectPtr<> Object : Objects)
	{
		GameplayTagContainerProperty = InKeyEditor.GetPropertyBindings()->GetProperty(*Object.Get());
		if (GameplayTagContainerProperty)
		{
			ISequencerPropertyKeyedStatusHandler& PropertyKeyedStatusHandler = InKeyEditor.GetSequencer()->GetPropertyKeyedStatusHandler();
			PropertyKeyedStatusHandler.GetExternalHandler(GameplayTagContainerProperty).BindRaw(this, &SGameplayTagContainerKeyEditor::GetPropertyKeyedStatus);
		}
	}
}

FGameplayTagContainer SGameplayTagContainerKeyEditor::GetGameplayTagContainer() const
{
	return KeyEditor.GetCurrentValue();
}

void SGameplayTagContainerKeyEditor::OnTagContainerChanged(const FGameplayTagContainer& InContainer)
{
	FScopedTransaction Transaction(LOCTEXT("SetGameplayTagContainerKey", "Set GameplayTagContainer Key Value"));
	KeyEditor.SetValueWithNotify(InContainer, EMovieSceneDataChangeType::TrackValueChangedRefreshImmediately);
}

EPropertyKeyedStatus SGameplayTagContainerKeyEditor::GetPropertyKeyedStatus(const IPropertyHandle& InPropertyHandle) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FGameplayTagContainerPropertyTrackEditor::GetPropertyKeyedStatus);

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
		UMovieSceneGameplayTagContainerTrack* GameplayTagContainerTrack = MovieScene->FindTrack<UMovieSceneGameplayTagContainerTrack>(OwingObjectHandle, InPropertyHandle.GetProperty()->GetFName());
		if (!GameplayTagContainerTrack || GameplayTagContainerTrack->IsEmpty())
		{
			continue;
		}
	
		// Check the key state in each section.
		for (const UMovieSceneSection* Section : GameplayTagContainerTrack->GetAllSections())
		{
			if (const UMovieSceneGameplayTagContainerSection* GameplayTagContainerSection = Cast<UMovieSceneGameplayTagContainerSection>(Section))
			{
				EPropertyKeyedStatus NewKeyedStatus = GetKeyedStatusInSection(*GameplayTagContainerSection, FrameRange);
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

EPropertyKeyedStatus SGameplayTagContainerKeyEditor::GetKeyedStatusInSection(const UMovieSceneGameplayTagContainerSection& InSection, const TRange<FFrameNumber>& InRange) const
{
	EPropertyKeyedStatus SectionKeyedStatus = EPropertyKeyedStatus::NotKeyed;
	int32 EmptyChannelCount = 0;

	TArrayView<FMovieSceneGameplayTagContainerChannel*> Channels = InSection.GetChannelProxy().GetChannels<FMovieSceneGameplayTagContainerChannel>();
	for (FMovieSceneGameplayTagContainerChannel* Channel : Channels)
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
