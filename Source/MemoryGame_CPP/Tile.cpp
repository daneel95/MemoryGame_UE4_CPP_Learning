// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "MemoryGame_CPPGameModeBase.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Find all elements needed
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BasicMaterial(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));

	// Some constants for my components
	FVector TileRelativeScale = FVector(1.0f, 1.0f, 0.2f);
	FVector FaceRelativeLocation = FVector(-0.4f, 0.0f, -54.0f);
	FRotator FaceRelativeRotation = FRotator(180.0f, 0.0f, 0.0f);

	// Setup Components
	// Setup Tile
	Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	Tile->SetRelativeScale3D(TileRelativeScale);
	if (CubeMeshAsset.Succeeded()) {
		Tile->SetStaticMesh(CubeMeshAsset.Object);
	}
	Tile->CastShadow = false;
	RootComponent = Tile;

	// Setup Face
	Face = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Face"));
	if (PlaneMeshAsset.Succeeded()) {
		Face->SetStaticMesh(PlaneMeshAsset.Object);
	}
	Face->SetRelativeLocation(FaceRelativeLocation);
	Face->SetRelativeRotation(FaceRelativeRotation);
	Face->AttachTo(Tile);

	if (BasicMaterial.Succeeded()) {
		Tile->SetMaterial(0, BasicMaterial.Object);
	}

	OnClicked.AddDynamic(this, &ATile::TileOnClickAction);

	// Timeline stuff
	RotationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeline"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Curves/RotationCurve.RotationCurve'"));
	if (Curve.Succeeded()) {
		RotationCurve = Curve.Object;
	}

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Tile Face
	InitializeTile();

	// Timeline stuff
	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;
	if (RotationCurve != nullptr) {
		RotationTimeline->SetLooping(false);
		RotationTimeline->SetTimelineLength(1.0f);
		RotationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		RotationTimeline->SetPlaybackPosition(0.0f, false);

		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		RotationTimeline->AddInterpFloat(RotationCurve, onTimelineCallback);
		RotationTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);
		RotationTimeline->RegisterComponent();
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Timeline Stuff
	if (RotationTimeline != nullptr) {
		RotationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
}

void ATile::InitializeTile() {
	FString FaceMaterialText;
	switch (Value) {
		case TileValuesEnum::BLACK_X:
			FaceMaterialText = TEXT("Material'/Game/Materials/X_Black_Mat.X_Black_Mat'");
			break;
		case TileValuesEnum::BLUE_X:
			FaceMaterialText = TEXT("Material'/Game/Materials/X_Blue_Mat.X_Blue_Mat'");
			break;
		case TileValuesEnum::RED_X:
			FaceMaterialText = TEXT("Material'/Game/Materials/X_Red_Mat.X_Red_Mat'");
			break;
		case TileValuesEnum::BLACK_O:
			FaceMaterialText = TEXT("Material'/Game/Materials/O_Black_Mat.O_Black_Mat'");
			break;
		case TileValuesEnum::BLUE_O:
			FaceMaterialText = TEXT("Material'/Game/Materials/O_Blue_Mat.O_Blue_Mat'");
			break;
		case TileValuesEnum::RED_O:
			FaceMaterialText = TEXT("Material'/Game/Materials/O_Red_Mat.O_Red_Mat'");
			break;
		case TileValuesEnum::BLACK_PLUS:
			FaceMaterialText = TEXT("Material'/Game/Materials/+_Black_Mat.+_Black_Mat'");
			break;
		case TileValuesEnum::BLUE_PLUS:
			FaceMaterialText = TEXT("Material'/Game/Materials/+_Blue_Mat.+_Blue_Mat'");
			break;
		case TileValuesEnum::RED_PLUS:
			FaceMaterialText = TEXT("Material'/Game/Materials/+_Red_Mat.+_Red_Mat'");
			break;
		default:
			FaceMaterialText = TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'");
			break;
	}

	// Get the material dynamically and set it up on the Face mesh
	UMaterialInterface* FaceMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *FaceMaterialText));
	Face->SetMaterial(0, FaceMaterial);

}

TEnumAsByte<TileValuesEnum> ATile::GetValue() {
	return Value;
}

void ATile::ForwardRotate() {
	if (RotationTimeline != nullptr) {
		RotationTimeline->PlayFromStart();
	}
}

void ATile::BackwardRotate() {
	if (RotationTimeline != nullptr) {
		RotationTimeline->ReverseFromEnd();
	}
}

void ATile::TileOnClickAction(AActor* TouchedActor, FKey ButtonPressed) {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clicked my tile"));

	float TileRoll = Tile->GetComponentRotation().Roll;
	bool bCanRotate = TileRoll <= 0.1f;

	if (bCanRotate) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Actually handling rotation"));
		AMemoryGame_CPPGameModeBase* MemoryGameGM = (AMemoryGame_CPPGameModeBase*)GetWorld()->GetAuthGameMode();
		if (MemoryGameGM->AddTurnedTile(this)) {
			ForwardRotate();
			MemoryGameGM->TryToHandleTwoTurnedTiles();
		}
	}
}

void ATile::TimelineCallback(float CurveValue) {
	FRotator TileRotator = Tile->GetComponentRotation();
	TileRotator.Roll = CurveValue;
	Tile->SetRelativeRotation(TileRotator);
}

void ATile::TimelineFinishedCallback() {
	// Do nothing
}

void ATile::SetValue(TEnumAsByte<TileValuesEnum> ValueToSet) {
	Value = ValueToSet;
}
