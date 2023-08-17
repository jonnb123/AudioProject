// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainWidget.h"
#include "Components/BoxComponent.h"
#include "AudioProjectMasteredCharacter.generated.h"


UCLASS(config=Game)
class AAudioProjectMasteredCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAudioProjectMasteredCharacter();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Points get/set
	int GetPoints() const { return Points; }
	void SetPoints(float NewValue) { Points = NewValue; }

	// Points get/set
	int GetIsCharacterDead() const { return Dead; }
	void SetIsCharacterDead(bool NewValue) { Dead = NewValue; }

	// Widget instances
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidgetClass;
	UUserWidget *MainWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> EndWidgetClass;
	UUserWidget *EndWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WinWidgetClass;
	UUserWidget *WinWidgetInstance;

	// functionality for character dying
	UFUNCTION(BlueprintCallable)
	void HandleCharacterDeath();

	// functionality for character winning
	UFUNCTION(BlueprintCallable)
	void HandleWin();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int Points = 0; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Dead = false; 

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

};

