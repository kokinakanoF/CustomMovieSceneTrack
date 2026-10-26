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
	/** SGameplayTagComboで表示するGameplayTagを返す */
	FGameplayTag GetGameplayTag() const;

	/** SGameplayTagComboの値が変更された時に呼び出される */
	void OnTagChanged(const FGameplayTag InTag);

private:
	/**
	 * プロパティに対するキーの設定状態を取得します
	 * 
	 * @param InPropertyHandle - 状態を確認する対象であるプロパティのハンドル
	 * @return キーの設定状態
	 */
	EPropertyKeyedStatus GetPropertyKeyedStatus(const IPropertyHandle& InPropertyHandle) const;

	/**
	 * セクション内のキーの設定状態を取得します
	 * 
	 * @param InSection - セクション
	 * @param InRange	- チェック範囲
	 * @return キーの設定状態
	 */
	EPropertyKeyedStatus GetKeyedStatusInSection(const class UMovieSceneGameplayTagSection& InSection, const TRange<FFrameNumber>& InRange) const;

private:
	TSequencerKeyEditor<FMovieSceneGameplayTagChannel, FGameplayTag> KeyEditor;
	FProperty* GameplayTagProperty = nullptr;
};