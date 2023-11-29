// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"
#include "TilePlayerController.h"

// Sets default values
ATileGameManager::ATileGameManager() :
	GridSize(100),
	GridOffset(0,0,0.5f),
	MapExtendsInGrids(16)

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	TilePreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TilePreview"));
	TilePreview->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> PreviewMaterial(TEXT("Material'/Game/Materials/M_Atlas_Tranclucent.M_Atlas_Tranclucent'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TilePreview->SetMaterial(0, PreviewMaterial.Object);
	TilePreview->SetRelativeScale3D(FVector(0.083333f, 0.083333f, 0.1f));
	TilePreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PlayerController = Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}
}

// Called every frame
void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	//No tile types?
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X = FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y = FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z = Location.Z;

	UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
		int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

		//Can not place out of the grid
		if (GridX <0 || GridY < 0 || GridX >= MapExtendsInGrids*2 || GridY >= MapExtendsInGrids*2)
		{
			UE_LOG(LogTemp, Error, TEXT("Out of Grid!"));
			return;
		}

		//Already a tile here?
		if (Map[GridX][GridY] != nullptr) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Place Tile Here!"));
			return;
		}

		//Place tile
		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;

			FTransform TileTransform(GridLoc + GridOffset);
			TileTransform = FTransform(TilePreview->GetRelativeRotation(), TileTransform.GetLocation(), TileTransform.GetScale3D());
			SelectedTile->InstancedMesh->AddInstance(SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform, true);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f, %f, %f"), HitActor ? *HitActor->GetActorLabel() : TEXT("None"), Location.X, Location.Y, Location.Z);
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		int previewRotation = 0; 
		previewRotation = (previewRotation + 90) % 360;
		TilePreview->SetRelativeRotation(FRotator(0, TilePreview->GetRelativeRotation().Yaw + previewRotation, 0));
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		if (CurrentTileIndex - 1 < 0)
		{
			CurrentTileIndex = TileTypes.Num() - 1;
		}
		else CurrentTileIndex--;

		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();


		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);

		TilePreview->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
		TilePreview->SetWorldLocation(GridLoc + GridOffset);
	}
}