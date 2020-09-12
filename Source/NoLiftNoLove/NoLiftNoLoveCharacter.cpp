// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NoLiftNoLoveCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "PaperSpriteComponent.h"
#include "Math/Rotator.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANoLiftNoLoveCharacter::ANoLiftNoLoveCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-45.0f, 0.0f, 0.0f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Set up Sprite Component
	SIMP = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SIMP"));
	SIMP->AttachTo(RootComponent);
	SIMP->SetWorldScale3D(FVector(4, 4, 4));
	SIMP->SetRelativeScale3D(FVector(4, 4, 4));
	SIMP->SetRelativeRotation(FRotator(0, 90, 0));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	IsWeightLifting = false;

	TotalTimeForWeightLifting = 1.125f;
	TotalTimeForDumbellLifting = 0.625f;
}



void ANoLiftNoLoveCharacter::StartLifting()
{
	//if player are not interacting, then meh :p
	if (IsInteractingBarbell) {
		IsWeightLifting = true;
		//SIMP->SetFlipbook(WeightLifting);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Is the code delaying? Before")));
		GetWorld()->GetTimerManager().SetTimer(TimerForLifting, this, &ANoLiftNoLoveCharacter::CheckIfStillLifting, TotalTimeForWeightLifting, false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Is the code delaying? After")));
	}
	if (IsInteractingDumbell) {
		IsDumbbellLifting = true;
		//SIMP->SetFlipbook(DumbbellLifting);
		//DumbellLiftingRep += 1;
		GetWorld()->GetTimerManager().SetTimer(TimerForLifting, this, &ANoLiftNoLoveCharacter::CheckIfStillLifting, TotalTimeForDumbellLifting, false);
	}
}

void ANoLiftNoLoveCharacter::RevertAnimation()
{

	//SIMP->SetFlipbook(Idle);
}


void ANoLiftNoLoveCharacter::CheckIfStillLifting()
{
	if (IsWeightLifting)
	{
		if (IsInteractingBarbell) {
			WeightLiftingRep += 1;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f"), x, y));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You finish one rep! Current Rep: %d"), WeightLiftingRep));
			IsWeightLifting = false;
			//SIMP->SetFlipbook(Idle);
		}
	}
	if (IsInteractingDumbell) {
		DumbellLiftingRep += 1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f"), x, y));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You finish one dumbell! Current Count: %d"), DumbellLiftingRep));
		IsWeightLifting = false;
		//SIMP->SetFlipbook(Idle);

	}

	

}



//Mouse Interrupt
void ANoLiftNoLoveCharacter::CheckIfLifting()
{
	if (IsWeightLifting )
	{
		IsWeightLifting = false;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Not Finish WeightLifting! Resetted IsWeightLifting")));
		GetWorldTimerManager().ClearTimer(TimerForLifting);
		//SIMP->SetFlipbook(Idle);
	}

	if (IsDumbbellLifting)
	{
		IsDumbbellLifting = false;
		GetWorldTimerManager().ClearTimer(TimerForLifting);
		//SIMP->SetFlipbook(Idle);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANoLiftNoLoveCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Lifting", IE_Pressed, this, &ANoLiftNoLoveCharacter::StartLifting);
	PlayerInputComponent->BindAction("Lifting", IE_Released, this, &ANoLiftNoLoveCharacter::CheckIfLifting);


	//PlayerInputComponent->BindAxis("MoveForward", this, &ANoLiftNoLoveCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANoLiftNoLoveCharacter::MoveRight);


}

void ANoLiftNoLoveCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !IsWeightLifting && !IsDumbbellLifting)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//left
		if (Value < 0)
		{
			SetActorRotation(FRotator(0.f, -180.f, 0.f));
		}
		//right
		else {
			SetActorRotation(FRotator(0.f, 0.f, 0.f));
		}
		AddMovementInput(Direction, Value);
	}
}

void ANoLiftNoLoveCharacter::MoveForward(float Value)
{
	//normal movement speed
	if ((Controller != NULL) && (Value != 0.0f))
	{

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);



		AddMovementInput(Direction, Value);
	}
}



