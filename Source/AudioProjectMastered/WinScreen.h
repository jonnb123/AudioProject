// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreen.generated.h"

/**
 *
 */
UCLASS()
class AUDIOPROJECTMASTERED_API UWinScreen : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel *CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *GameOverText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *FinalPointsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *RestartGameText;

	UPROPERTY(meta = (BindWidget))
	class UButton *RestartGameButton;

	void NativeConstruct() override;

	UFUNCTION()
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	FText UpdateCharacterPoints();
};
