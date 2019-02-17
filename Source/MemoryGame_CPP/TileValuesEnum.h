// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum TileValuesEnum 
{
	BLACK_PLUS	UMETA(DisplayName = "BlackPlus"),
	RED_PLUS	UMETA(DisplayName = "RedPlus"),
	BLUE_PLUS	UMETA(DisplayName = "BluePlus"),
	BLACK_X		UMETA(DisplayName = "BlackX"),
	RED_X		UMETA(DisplayName = "RedX"),
	BLUE_X		UMETA(DisplayName = "BlueX"),
	BLACK_O		UMETA(DisplayName = "BlackO"),
	RED_O		UMETA(DisplayName = "RedO"),
	BLUE_O		UMETA(DisplayName = "BlueO")
};

class TileEnumAuxiliary {
public:
	const static int32 NumberOfEnumValues = 9;
};