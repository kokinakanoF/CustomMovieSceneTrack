// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "CurveKeyEditors/SequencerKeyEditor.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "GameplayTagContainer.h"
#include "GameplayTag/MovieSceneGameplayTagChannel.h"

/**
 * A widget for editing a curve representing string keys.
 */
class SGameplayTagCurveKeyEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameplayTagCurveKeyEditor) {}
	SLATE_END_ARGS();

	~SGameplayTagCurveKeyEditor();

	void Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag>& InKeyEditor);

private:

	/** Returns the GameplayTag displayed by SGameplayTagCombo. */
	FGameplayTag GetGameplayTag() const;

	/** Called when the value of SGameplayTagCombo changes. */
	void OnTagChanged(const FGameplayTag InTag);

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
	EPropertyKeyedStatus GetKeyedStatusInSection(const class UMovieSceneGameplayTagSection& InSection, const TRange<FFrameNumber>& InRange) const;

private:

	TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag> KeyEditor;

	/**
	 * Pointer to the property whose type is GameplayTag.
	 * Used to unbind the callback in the destructor.
	 */
	FProperty* GameplayTagProperty = nullptr;
};