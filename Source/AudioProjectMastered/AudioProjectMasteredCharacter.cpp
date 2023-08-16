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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AAudioProjectMasteredCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	if (WinWidgetClass)
	{
		if (WinWidgetInstance)
		{
			WinWidgetInstance->RemoveFromParent();
		}
	}

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

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAudioProjectMasteredCharacter::Move);
	}
}

void AAudioProjectMasteredCharacter::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

		// add movement in the Z-axis (up and down)
		AddMovementInput(UpDirection, MovementVector.Y);
	}
}

void AAudioProjectMasteredCharacter::Death()
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

void AAudioProjectMasteredCharacter::Win()
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

// void AAudioProjectMasteredCharacter::Look(const FInputActionValue& Value)
// {
// 	// input is a Vector2D
// 	FVector2D LookAxisVector = Value.Get<FVector2D>();

// 	if (Controller != nullptr)
// 	{
// 		// add yaw and pitch input to controller
// 		AddControllerYawInput(LookAxisVector.X);
// 		AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }
