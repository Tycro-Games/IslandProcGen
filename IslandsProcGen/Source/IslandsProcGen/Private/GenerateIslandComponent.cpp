// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateIslandComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"


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

	// ...
	// Try to find the TileMapComponent in the owner actor
	TileMapComponent = GetOwner()->FindComponentByClass<UPaperTileMapComponent>();
	if (TileMapComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileMapComponent not found!"));
		return;
	}

	// Call the function to get all tiles
	GetAllTiles();
}

void UGenerateIslandComponent::GetAllTiles()
{
	if (TileMapComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileMapComponent is null!"));
		return;
	}

	UPaperTileMap* TileMap = TileMapComponent->TileMap;
	if (TileMap == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileMap is null!"));
		return;
	}

	for (int32 LayerIndex = 0; LayerIndex < TileMap->TileLayers.Num(); ++LayerIndex)
	{
		UPaperTileLayer* TileLayer = TileMap->TileLayers[LayerIndex];
		if (TileLayer == nullptr)
		{
			continue;
		}

		for (int32 Y = 0; Y < TileMap->MapHeight; ++Y)
		{
			for (int32 X = 0; X < TileMap->MapWidth; ++X)
			{
				FPaperTileInfo Tile = TileLayer->GetCell(X, Y);
				// Process the tile info as needed
				UE_LOG(LogTemp, Log, TEXT("Layer: %d, Tile: (%d, %d), TileSet: %s, TileIndex: %d"),
				       LayerIndex, X, Y,
				       Tile.TileSet ? *Tile.TileSet->GetName() : TEXT("None"),
				       Tile.GetTileIndex());
			}
		}
	}
}


// Called every frame
void UGenerateIslandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
