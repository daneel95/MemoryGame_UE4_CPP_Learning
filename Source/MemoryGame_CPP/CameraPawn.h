// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CameraPawn.generated.h"

UCLASS()
class MEMORYGAME_CPP_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();
	UCameraComponent* Camera;
};
