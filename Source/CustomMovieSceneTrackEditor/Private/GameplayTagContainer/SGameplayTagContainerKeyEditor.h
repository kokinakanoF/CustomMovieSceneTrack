// Copyright Koki Nakano. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "CurveKeyEditors/SequencerKeyEditor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "GameplayTagContainer.h"
#include "GameplayTagContainer/MovieSceneGameplayTagContainerChannel.h"

/**
 * A widget for editing a curve representing string keys.
 */
class SGameplayTagContainerKeyEditor : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGameplayTagContainerKeyEditor) {}
	SLATE_END_ARGS();

	~SGameplayTagContainerKeyEditor();

	void Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagContainerChannel, FGameplayTagContainer>& InKeyEditor);

private:

	/** Returns the GameplayTag displayed by SGameplayTagCombo. */
	FGameplayTagContainer GetGameplayTagContainer() const;

	/** Called when the value of SGameplayTagCombo changes. */
	void OnTagContainerChanged(const FGameplayTagContainer& InContainer);

private:

	/**
	 * Gets the key state for a property.
	 * 
	 * @param InPropertyHandle	Handle for the property whose state is checked.
	 * @return The key state.
	 */
	EPropertyKeyedStatus GetPropertyKeyedStatus(const IPropertyHandle& InPropertyHandle) const;

	/**
	 * Gets the key state within a section.
	 * 
	 * @param InSection	Section.
	 * @param InRange	Range to check.
	 * @return The key state.
	 */
	EPropertyKeyedStatus GetKeyedStatusInSection(const class UMovieSceneGameplayTagContainerSection& InSection, const TRange<FFrameNumber>& InRange) const;

private:

	TSequencerKeyEditor<FMovieSceneGameplayTagContainerChannel, FGameplayTagContainer> KeyEditor;

	/**
	 * Pointer to the property whose type is GameplayTag.
	 * Used to unbind the callback in the destructor.
	 */
	FProperty* GameplayTagContainerProperty = nullptr;
};