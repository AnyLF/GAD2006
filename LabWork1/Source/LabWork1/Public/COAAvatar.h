// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class LABWORK1_API ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()

public:

	ACOAAvatar();

	UPROPERTY(EditAnywhere, Category = "COA")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "COA")
	UCameraComponent* Camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float RunningSpeed;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "COA")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaGainRate;

	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bHoldingRunKey;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bStaminaDrained;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForwad(float Amount);
	void MoveRight(float Amount);

	void RunPressed();
	void RunReleased();

	void UpdateMovementParams();
};
