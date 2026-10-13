// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGameplayTagCurveKeyEditor.h"
#include "SGameplayTagCombo.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "SGameplayTagCurveKeyEditor"

void SGameplayTagCurveKeyEditor::Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag>& InKeyEditor)
{
	KeyEditor = InKeyEditor;

	ChildSlot
	[
		SNew(SGameplayTagCombo)
		//.MinDesiredWidth(10.f)
		//.SelectAllTextWhenFocused(true)
		.Tag(this, &SGameplayTagCurveKeyEditor::GetGameplayTag)
		.OnTagChanged(this, &SGameplayTagCurveKeyEditor::OnTagChanged)
	];
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

#undef LOCTEXT_NAMESPACE
