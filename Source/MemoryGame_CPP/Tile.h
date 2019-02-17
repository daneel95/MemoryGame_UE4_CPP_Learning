// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileValuesEnum.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "Tile.generated.h"

UCLASS()
class MEMORYGAME_CPP_API ATile : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<TileValuesEnum> Value;
	//TileValuesEnum Value;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Tile;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Face;

	// Rotation stuff (Timeline)
	UTimelineComponent* RotationTimeline;
	UPROPERTY()
	UCurveFloat* RotationCurve;
	FOnTimelineFloat InterpFunction{};

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TEnumAsByte<TileValuesEnum> GetValue();
	void SetValue(TEnumAsByte<TileValuesEnum> ValueToSet);
	// Initialize Tile - Change Face material according to Value
	void InitializeTile();

	// Timeline stuff
	UFUNCTION()
	void ForwardRotate();
	UFUNCTION()
	void BackwardRotate();
	UFUNCTION()
	void TimelineCallback(float CurveValue);
	UFUNCTION()
	void TimelineFinishedCallback();
	UFUNCTION()
	void TileOnClickAction(AActor* TouchedActor, FKey ButtonPressed);
};
