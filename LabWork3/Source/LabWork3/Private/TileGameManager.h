// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.h"
#include "TileGameManager.generated.h"

#define MAX_MAP_GRID_SIZE 100

UCLASS()
class ATileGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGameManager();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GridSelection;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TilePreview;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed);

	UPROPERTY(EditAnywhere)
	int GridSize;
	
	UPROPERTY(EditAnywhere)
	FVector GridOffset;
	
	UPROPERTY(EditAnywhere)
	TArray<ATileBase*> TileTypes;
	
	UPROPERTY(EditAnywhere)
	int MapExtendsInGrids;
	
	int CurrentTileIndex;

	ATileBase* Map[MAX_MAP_GRID_SIZE][MAX_MAP_GRID_SIZE];

};
