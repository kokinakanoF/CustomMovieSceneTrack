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

	void Construct(const FArguments& InArgs, const TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag>& InKeyEditor);

private:
	/** SGameplayTagComboで表示するGameplayTagを返す */
	FGameplayTag GetGameplayTag() const;

	/** SGameplayTagComboの値が変更された時に呼び出される */
	void OnTagChanged(const FGameplayTag InTag);

private:
	TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag> KeyEditor;
};