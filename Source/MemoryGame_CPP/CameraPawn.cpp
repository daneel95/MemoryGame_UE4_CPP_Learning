// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn.h"

ACameraPawn::ACameraPawn() {
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	RootComponent = Camera;
}
