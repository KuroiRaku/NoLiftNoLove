// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NoLiftNoLoveCharacter.generated.h"


UENUM()
enum class EObjectHolding
{
	DUMBELL,
	WEIGHTLIFTER
};
UCLASS(config=Game)
class ANoLiftNoLoveCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	void MoveForward(float Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* SIMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* WeightLifting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* Dropdown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* DumbellLifting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int WeightLiftingRep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float TotalTimeForWeightLifting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int DumbellLiftingRep;

	//we only implement stats on fullgame :p
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int Strength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int Flexibility;

	//Stats affect story
	//Tread Mill will increase stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int Stamina;

public:


	ANoLiftNoLoveCharacter();

	FTimerHandle TimerForLifting;

	bool IsWeightLifting;

	void StartLifting();

	void CheckIfLifting();

	void CheckIfStillLifting();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
