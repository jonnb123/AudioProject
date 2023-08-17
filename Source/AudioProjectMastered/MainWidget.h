// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class AUDIOPROJECTMASTERED_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel *CanvasPanel;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *PointsText;

	UFUNCTION(BlueprintCallable)
	FText UpdateCharacterPoints();

};
