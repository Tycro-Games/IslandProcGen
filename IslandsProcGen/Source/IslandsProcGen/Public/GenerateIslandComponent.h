// Fill out your copyright notice in the Description page of Project Settings.
// used chatgpt
#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"

#include "GenerateIslandComponent.generated.h"

// Struct definition
USTRUCT(BlueprintType)
struct FCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
	FVector Position;

	// Constructors
	FCell()
	{
	}

	bool operator==(const FCell& Other) const
	{
		return Position == Other.Position;
	}

	FCell(const FVector& InLocation)
		: Position(InLocation)
	{
	}


	// Necessary for using TMap with FGridCell as a key
	friend uint32 GetTypeHash(const FCell& Cell)
	{
		return GetTypeHash(Cell.Position);
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ISLANDSPROCGEN_API UGenerateIslandComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DrawGrid(const FVector& Origin, int32 NumCellsX, int32 NumCellsY, float CellSize);
	// Sets default values for this component's properties
	UGenerateIslandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void GetAllTiles();
	void LoadCSVFile(const FString& FileName, TMap<FCell, int32>& OutGridCells);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Mesh")
	TObjectPtr<UStaticMesh> StaticMeshComponent;

	// Dictionary to hold grid cells
	UPROPERTY(EditAnywhere, Category = "Grid")
	TMap<FCell, int32> GridCells;
	UPROPERTY(EditAnywhere, Category = "Grid")

	FIntVector2 GridSize = {10, 10};
	UPROPERTY(EditAnywhere, Category = "Grid")

	float SizePerCell = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Grid")

	FString TilePath = "island1.tmx";

private:
};
