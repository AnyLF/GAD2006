// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"


ACOAAvatar::ACOAAvatar():
	RunningSpeed(600.0f),
	Stamina(100.0f),
	MaxStamina(100.0f),
	StaminaGainRate(30.0f),
	StaminaDrainRate(50.0f),
	bHoldingRunKey(false),
	bStaminaDrained(false)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();

	Stamina = MaxStamina;
}


void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update Stamina
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking) 
	{
		if (!bHoldingRunKey || bStaminaDrained)
		{
			Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);

			if (Stamina >= MaxStamina)
			{
				bStaminaDrained = false;
				UpdateMovementParams();
			}
		}
		else if (!GetCharacterMovement()->Velocity.IsNearlyZero(0.01f))
		{
			Stamina = FMath::Max(0.0f, Stamina - StaminaDrainRate * DeltaTime);
			if (Stamina == 0.0f)
			{
				bStaminaDrained = true;
				UpdateMovementParams();
			}
		}
		
	}

	GEngine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Stamina: %f"), Stamina));

	UE_LOG(LogTemp, Warning, TEXT("Stamina: %f"), Stamina); //print stringing but only showed in the output window

}


void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForwad", this, &ACOAAvatar::MoveForwad);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::MoveForwad(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, Amount);
}

void ACOAAvatar::MoveRight(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector RightDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, Amount);
}

void ACOAAvatar::RunPressed()
{
	bHoldingRunKey = true;
	UpdateMovementParams();
}

void ACOAAvatar::RunReleased()
{
	bHoldingRunKey = false;
	UpdateMovementParams();
}

void ACOAAvatar::UpdateMovementParams()
{
	GetCharacterMovement()->MaxWalkSpeed =
		bHoldingRunKey && !bStaminaDrained ? RunningSpeed : WalkingSpeed;
}