// Copyright Epic Games, Inc. All Rights Reserved.

#include "AudioProjectMasteredCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//////////////////////////////////////////////////////////////////////////
// AAudioProjectMasteredCharacter

AAudioProjectMasteredCharacter::AAudioProjectMasteredCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;								// Camera does not rotate relative to arm

}

void AAudioProjectMasteredCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		// Create the widget and store the instance in the variable.
		MainWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);

		// Check if the created widget instance is valid before adding it to the viewport.
		if (MainWidgetInstance)
		{
			// Add the widget to the viewport.
			MainWidgetInstance->AddToViewport();
		}
	}

	// Add Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAudioProjectMasteredCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAudioProjectMasteredCharacter::Move);
	}
}

void AAudioProjectMasteredCharacter::Move(const FInputActionValue &Value)
{
    // Extract movement input from the provided FInputActionValue.
    FVector2D MovementVector = Value.Get<FVector2D>();

    // Check if the character has a valid controller.
    if (Controller != nullptr)
    {
        // Get the current control rotation of the character's controller.
        const FRotator Rotation = Controller->GetControlRotation();

        // Create a rotation with yaw from Rotation and pitch/roll set to 0.
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Calculate the local up direction in the world based on YawRotation.
        const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

        // Add movement input in the Z-axis (up and down) using UpDirection and MovementVector.Y.
        // MovementVector.Y is positive for upward movement and negative for downward movement.
        AddMovementInput(UpDirection, MovementVector.Y);
    }
}


void AAudioProjectMasteredCharacter::HandleCharacterDeath()
{
	if (MainWidgetInstance)
	{
		MainWidgetInstance->RemoveFromParent();
		MainWidgetInstance = nullptr;
	}
	// Make sure to check if the widget class is valid before creating it.
	if (EndWidgetClass)
	{
		// Create the widget and store the instance in the variable.
		EndWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), EndWidgetClass);

		// Check if the created widget instance is valid before adding it to the viewport.
		if (EndWidgetInstance)
		{
			// Add the widget to the viewport.
			EndWidgetInstance->AddToViewport();
		}
	}
	GetCharacterMovement()->StopMovementImmediately();
	GetMesh()->SetVisibility(false);
	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	GetCharacterMovement()->MaxFlySpeed = 0;
	Dead = true;
}

void AAudioProjectMasteredCharacter::HandleWin()
{
	if (MainWidgetInstance)
	{
		MainWidgetInstance->RemoveFromParent();
		MainWidgetInstance = nullptr;
	}
	// Make sure to check if the widget class is valid before creating it.
	if (WinWidgetClass)
	{
		// Create the widget and store the instance in the variable.
		WinWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass);

		// Check if the created widget instance is valid before adding it to the viewport.
		if (WinWidgetInstance)
		{
			// Add the widget to the viewport.
			WinWidgetInstance->AddToViewport();
		}
	}
	GetCharacterMovement()->StopMovementImmediately();
	GetMesh()->SetVisibility(false);
	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	GetCharacterMovement()->MaxFlySpeed = 0;
}
