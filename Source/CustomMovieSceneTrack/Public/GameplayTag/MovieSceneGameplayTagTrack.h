// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "MovieSceneGameplayTagTrack.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMMOVIESCENETRACK_API UMovieSceneGameplayTagTrack : public UMovieScenePropertyTrack
{
	GENERATED_BODY()
	
public:
	UMovieSceneGameplayTagTrack(const FObjectInitializer& ObjInit);
	  
	//~Begin UMovieSceneTrack interface
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
};
