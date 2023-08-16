// Fill out your copyright notice in the Description page of Project Settings.


#include "WinScreen.h"
#include "AudioProjectMasteredCharacter.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWinScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &UWinScreen::RestartGame);
    }
}

void UWinScreen::RestartGame()
{
    // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AAudioProjectMasteredCharacter *Character = Cast<AAudioProjectMasteredCharacter>(PlayerCharacter);
    // Open the level by name (replace "YourLevelName" with the actual name of the level you want to open)
    RemoveFromParent();
    UGameplayStatics::OpenLevel(PlayerController, FName("ThirdPersonMap"));
    PlayerController->SetInputMode(FInputModeGameOnly());
}

FText UWinScreen::UpdateCharacterPoints()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AAudioProjectMasteredCharacter *Character = Cast<AAudioProjectMasteredCharacter>(PlayerCharacter);
    if (Character)
    {
        FString Text = FString::Printf(TEXT("%d"), Character->Points);
        return FText::FromString(Text);
    }
    else
    {
        return FText::FromString("N/A");
    }
}
