// Fill out your copyright notice in the Description page of Project Settings.
#include "MainWidget.h"
#include "AudioProjectMasteredCharacter.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

FText UMainWidget::UpdateCharacterPoints()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AAudioProjectMasteredCharacter *Character = Cast<AAudioProjectMasteredCharacter>(PlayerCharacter);
    if (Character)
    {
        FString Text = FString::Printf(TEXT("%d"), Character->GetPoints());
        return FText::FromString(Text);
    }
    else
    {
        return FText::FromString("N/A");
    }
}

