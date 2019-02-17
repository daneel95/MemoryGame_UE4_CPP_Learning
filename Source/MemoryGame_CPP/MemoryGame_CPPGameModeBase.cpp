// Fill out your copyright notice in the Description page of Project Settings.

#include "MemoryGame_CPPGameModeBase.h"
#include "CameraPawn.h"
#include "Tile.h"
#include "EngineUtils.h"
#include "TileValuesEnum.h"
#include "MemoryGame_PlayerController.h"


AMemoryGame_CPPGameModeBase::AMemoryGame_CPPGameModeBase() {
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ACameraPawn::StaticClass();
	PlayerControllerClass = AMemoryGame_PlayerController::StaticClass();
	// Used for delay version
	//bAlreadyHandlingTiles = false;
}

bool AMemoryGame_CPPGameModeBase::AddTurnedTile(ATile* Tile) {
	bool bCanAdd = TurnedTiles.Num() < 2;
	if (bCanAdd) {
		TurnedTiles.Add(Tile);
	}

	return bCanAdd;
}


// Called every frame
void AMemoryGame_CPPGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handling Turned tiles using delay
	//bool bEnoughTurnedTiles = TurnedTiles.Num() >= 2;
	//if (bEnoughTurnedTiles && !bAlreadyHandlingTiles) {
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Got 2 tiles. Can handle them"));
		//HandleTwoTurnedTiles();
	//	bAlreadyHandlingTiles = true; // the two tiles are already being handled. Don't try to handle them again
	//	GetWorldTimerManager().SetTimer(DelayTimerHandler, this, &AMemoryGame_CPPGameModeBase::HandleTwoTurnedTiles, 1.0f, false);

	//}
}

void AMemoryGame_CPPGameModeBase::HandleTwoTurnedTiles() {
	ATile* Tile1 = TurnedTiles[0];
	ATile* Tile2 = TurnedTiles[1];
	bool bAreEqual = Tile1->GetValue() == Tile2->GetValue();

	if (bAreEqual) {
		Tile1->Destroy();
		Tile2->Destroy();
	} else {
		Tile1->BackwardRotate();
		Tile2->BackwardRotate();
	}

	TurnedTiles.Empty();
}

void AMemoryGame_CPPGameModeBase::TryToHandleTwoTurnedTiles() {
	bool bEnoughTurnedTiles = TurnedTiles.Num() >= 2;
	if (bEnoughTurnedTiles) {
		// If there are 2 tiles then handle them after 1 second.
		GetWorldTimerManager().SetTimer(DelayTimerHandler, this, &AMemoryGame_CPPGameModeBase::HandleTwoTurnedTiles, 1.0f, false);
	}
}

void AMemoryGame_CPPGameModeBase::RandomInitialization() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Starting random initialization"));
	TArray<ATile*> GameTiles;
	TArray<TEnumAsByte<TileValuesEnum>> TilesValues;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shuffle arrays"));
	for (TActorIterator<ATile>TileIterator(GetWorld()); TileIterator; ++TileIterator) {
		GameTiles.Add(*TileIterator);
	}

	int32 NumberOfEnumValues = TileEnumAuxiliary::NumberOfEnumValues;
	for (int32 i = 0; i < NumberOfEnumValues; i++) {
		TilesValues.Add(static_cast<TEnumAsByte<TileValuesEnum>>(i));
	}
	
	GameTiles = ShuffleArray(GameTiles);
	TilesValues = ShuffleArray(TilesValues);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Setting values"));
	for (int32 i = 0; i < GameTiles.Num(); i++) {
		TEnumAsByte<TileValuesEnum> ValueToSet = TilesValues[i / 2];
		GameTiles[i]->SetValue(ValueToSet);
		GameTiles[i]->InitializeTile();
	}
}

template <typename AnyObject>
TArray<AnyObject> AMemoryGame_CPPGameModeBase::ShuffleArray(TArray<AnyObject> ArrayToShuffle) {
	int32 ArraySize = ArrayToShuffle.Num();
	TArray<AnyObject> ShuffledArray;
	for (int32 i = 0; i < ArraySize; i++) {
		int32 index = FMath::RandRange(0, ArraySize - i - 1);
		ShuffledArray.Add(ArrayToShuffle[index]);
		ArrayToShuffle.RemoveAt(index);
	}
	return ShuffledArray;
}

void AMemoryGame_CPPGameModeBase::BeginPlay() {
	Super::BeginPlay();
	RandomInitialization();
}
