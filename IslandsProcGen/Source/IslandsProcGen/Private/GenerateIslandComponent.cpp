// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateIslandComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#include "DrawDebugHelpers.h"


void UGenerateIslandComponent::DrawGrid(const FVector& Origin, int32 NumCellsX, int32 NumCellsY,
                                        float CellSize)
{
	FVector LineStart, LineEnd;

	// Draw horizontal lines
	for (int32 Y = 0; Y <= NumCellsY; Y++)
	{
		LineStart = FVector(Origin.X, Origin.Y + Y * CellSize, Origin.Z);
		LineEnd = FVector(Origin.X + NumCellsX * CellSize, Origin.Y + Y * CellSize, Origin.Z);
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Purple, true, -1, 0, 10
		);
	}

	// Draw vertical lines
	for (int32 X = 0; X <= NumCellsX; X++)
	{
		LineStart = FVector(Origin.X + X * CellSize, Origin.Y, Origin.Z);
		LineEnd = FVector(Origin.X + X * CellSize, Origin.Y + NumCellsY * CellSize, Origin.Z);
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Purple, true, -1, 0, 10
		);
	}
}

// Sets default values for this component's properties
UGenerateIslandComponent::UGenerateIslandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGenerateIslandComponent::BeginPlay()
{
	Super::BeginPlay();
	GridCells.Reserve(GridSize.X * GridSize.Y);
	int sizeX = GridSize.X;
	for (int x = 0; x < GridSize.X; x++)
	{
		for (int y = 0; y < GridSize.Y; y++)
		{
			FVector2D GridCell(x, y);
			FCell cell{FVector{static_cast<double>(x), static_cast<double>(y), 0}};
			GridCells.Add(cell, 0);
		}
	}
	DrawGrid(FVector{0, 0, 0}, GridSize.X, GridSize.Y, SizePerCell);
}

void UGenerateIslandComponent::GetAllTiles()
{
}


// Called every frame
void UGenerateIslandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
