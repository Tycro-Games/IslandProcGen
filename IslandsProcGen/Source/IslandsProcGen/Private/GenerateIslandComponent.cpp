// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateIslandComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

#include "DrawDebugHelpers.h"
#include "XmlFile.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Editor/EditorEngine.h"


void UGenerateIslandComponent::DrawGrid(const FVector& Origin, int32 NumCellsX, int32 NumCellsY,
                                        float CellSize)
{
	FlushPersistentDebugLines(GEditor->GetEditorWorldContext().World());
	FVector LineStart, LineEnd;

	// Draw horizontal lines
	for (int32 Y = 0; Y <= NumCellsY; Y++)
	{
		LineStart = FVector(Origin.X, Origin.Y + Y * CellSize, Origin.Z);
		LineEnd = FVector(Origin.X + NumCellsX * CellSize, Origin.Y + Y * CellSize, Origin.Z);
		DrawDebugLine(GEditor->GetEditorWorldContext().World(), LineStart, LineEnd, FColor::Purple, true, -1, 0, 10
		);
	}

	// Draw vertical lines
	for (int32 X = 0; X <= NumCellsX; X++)
	{
		LineStart = FVector(Origin.X + X * CellSize, Origin.Y, Origin.Z);
		LineEnd = FVector(Origin.X + X * CellSize, Origin.Y + NumCellsY * CellSize, Origin.Z);
		DrawDebugLine(GEditor->GetEditorWorldContext().World(), LineStart, LineEnd, FColor::Purple, true, -1, 0, 10
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


void UGenerateIslandComponent::GenerateIsland()
{
	for (UChildActorComponent* ChildComponentActor : ChildComponentActors)
	{
		if (ChildComponentActor && !ChildComponentActor->IsBeingDestroyed())
		{
			ChildComponentActor->DestroyComponent();
		}
	}
	ChildComponentActors.Empty();


	LoadCSVFile(TilePath, GridCells);

	DrawGrid(GetOwner()->GetActorLocation(), GridSize.X, GridSize.Y, SizePerCell);
	GetAllTiles();
}

// Called when the game starts
void UGenerateIslandComponent::BeginPlay()
{
	Super::BeginPlay();
}

//void UGenerateIslandComponent::GetAllTiles()
//{
//	AActor* ParentActor = GetOwner();
//	for (const auto& Elem : GridCells)
//	{
//		// Assuming FCell has a method to get the world position
//		FVector SpawnLocation = Elem.Key.Position * SizePerCell + SizePerCell / 2.0f;
//		FRotator SpawnRotation = FRotator::ZeroRotator;
//		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
//
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.Owner = ParentActor;
//
//		UChildActorComponent* NewActor = NewObject<UChildActorComponent>(ParentActor);
//
//
//		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
//		//NewActor->AttachToActor(ParentActor, AttachmentRules);
//		//// Add a StaticMeshComponent to the spawned actor
//
//		// Attach the new actor to the parent actor
//		NewActor->RegisterComponent();
//		NewActor->SetWorldTransform(SpawnTransform);
//
//		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(NewActor);
//
//		StaticMeshComponent->AttachToComponent(NewActor, AttachmentRules);
//		//// Set the mesh for the StaticMeshComponent
//		StaticMeshComponent->RegisterComponent();
//		StaticMeshComponent->SetRelativeLocation(FVector{0});
//		if (Elem.Value != 0)
//		{
//			// Spawn the actor
//
//			if (NewActor)
//			{
//				StaticMeshComponent->SetStaticMesh(StaticMesh);
//			}
//		}
//	}
//}

void UGenerateIslandComponent::GetAllTiles()
{
	AActor* ParentActor = GetOwner();
	for (const auto& Elem : GridCells)
	{
		if (Elem.Value != 0)
		{
			// Assuming FCell has a method to get the world position
			FVector SpawnLocation = Elem.Key.Position * SizePerCell + SizePerCell / 2.0f;
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = ParentActor;

			CreateChildActor(SpawnTransform);
		}
	}
}

void UGenerateIslandComponent::CreateChildActor(FTransform Transform)
{
	AActor* ParentActor = GetOwner();

	UChildActorComponent* ChildActorComponent = NewObject<UChildActorComponent>(ParentActor);
	if (ChildActorComponent)
	{
		ChildComponentActors.Add(ChildActorComponent);
		ChildActorComponent->RegisterComponent();
		ChildActorComponent->SetChildActorClass(ChildActorClass);
		if (ChildActorComponent->AttachToComponent(ParentActor->GetRootComponent(),
		                                           FAttachmentTransformRules::KeepRelativeTransform))
		{
			ChildActorComponent->SetRelativeTransform(Transform);
			ChildActorComponent->CreateChildActor();
		}
	}
}

void UGenerateIslandComponent::LoadCSVFile(const FString& FileName, TMap<FCell, int32>& OutGridCells)
{
	// Construct the full file path
	const FString FilePath = FPaths::ProjectContentDir() / FileName;

	// Load the XML file
	FXmlFile XmlFile(FilePath);

	if (!XmlFile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load XML file: %s"), *FilePath);
		return;
	}

	FXmlNode* RootNode = XmlFile.GetRootNode();
	if (!RootNode)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get root node"));
		return;
	}

	FXmlNode* LayerNode = RootNode->FindChildNode(TEXT("layer"));
	if (!LayerNode)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find layer node"));
		return;
	}

	FXmlNode* DataNode = LayerNode->FindChildNode(TEXT("data"));
	if (!DataNode)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find data node"));
		return;
	}

	FString CsvData = DataNode->GetContent();
	TArray<TArray<int32>> ParsedData;

	TArray<FString> Rows;
	CsvData.ParseIntoArray(Rows, TEXT("\n"), true);

	for (const FString& Row : Rows)
	{
		TArray<FString> Columns;
		Row.ParseIntoArray(Columns, TEXT(","), true);

		TArray<int32> ParsedRow;
		for (const FString& Column : Columns)
		{
			ParsedRow.Add(FCString::Atoi(*Column));
		}

		ParsedData.Add(ParsedRow);
	}
	GridSize.X = Rows.Num();
	GridSize.Y = ParsedData.Num();
	for (int32 RowIndex = 0; RowIndex < ParsedData.Num(); ++RowIndex)
	{
		FString RowString;
		for (int32 ColIndex = 0; ColIndex < ParsedData[RowIndex].Num(); ++ColIndex)
		{
			int32 Value = ParsedData[RowIndex][ColIndex];
			RowString += FString::Printf(TEXT("%d "), Value);
			FCell Cell(FVector{static_cast<double>(RowIndex), static_cast<double>(ColIndex), 0});
			OutGridCells.Add(Cell, Value);
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *RowString);
	}
}


// Called every frame
void UGenerateIslandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
