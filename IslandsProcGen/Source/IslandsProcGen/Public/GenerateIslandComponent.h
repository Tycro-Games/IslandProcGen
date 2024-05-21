// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "GenerateIslandComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ISLANDSPROCGEN_API UGenerateIslandComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGenerateIslandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void GetAllTiles();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPaperTileMapComponent* TileMapComponent;
	UPROPERTY(EditAnywhere)
	UPaperTileSet* TileSet;
};
