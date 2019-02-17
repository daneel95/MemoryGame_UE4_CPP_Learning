// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tile.h"
#include "MemoryGame_CPPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYGAME_CPP_API AMemoryGame_CPPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AMemoryGame_CPPGameModeBase();
	bool AddTurnedTile(ATile* Tile);
	void HandleTwoTurnedTiles();
	void TryToHandleTwoTurnedTiles();
	template <typename AnyObject>
	TArray<AnyObject> ShuffleArray(TArray<AnyObject> ArrayToShuffle);

private:
	TArray<ATile*> TurnedTiles;
	FTimerHandle DelayTimerHandler;
	//For delay version
	//bool bAlreadyHandlingTiles;
	void RandomInitialization();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
