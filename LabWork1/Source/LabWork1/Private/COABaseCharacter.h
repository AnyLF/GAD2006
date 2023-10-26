// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "COABaseCharacter.generated.h"

UCLASS()
class ACOABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACOABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bDeath;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float HealingRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, Category = "COA")
	bool Update;

	UFUNCTION(BlueprintCallable, Category = "COA")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "COA")
	float GetHealth();

};
