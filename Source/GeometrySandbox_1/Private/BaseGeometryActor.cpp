// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGeometryActor.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

#include "Materials/MaterialInstanceDynamic.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All);

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()

{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	PrintTransform();
	SetColor(GeometryData.Color);


	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFierd, GeometryData.TimerRate, true);
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{ 
	Super::Tick(DeltaTime);
	HandleMovement();

}

void ABaseGeometryActor::PrintTypes()
{

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name : %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Weapons num: %i, kills num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health num: %f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Is Dead : %d"), IsDead);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Has Weapon : %d"), static_cast<int>(HasWeapon));
}

void ABaseGeometryActor::PrintStringTypes()
{
	FString Name = "John Connor";
	UE_LOG(LogBaseGeometry, Display, TEXT("Name: %s"), *Name);

	PrintTypes();

	FString WeaponsNumStr = " Weapons num =" + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health =" + FString::SanitizeFloat(Health);
	FString IsDeadStr = "Is dead =" + FString(IsDead ? "true" : "false");
	FString Stat = FString::Printf(TEXT("\n -- All Stat --\n %s \n %s \n %s"), *WeaponsNumStr, *HealthStr, *IsDeadStr);

	UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *Stat);

	GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Red, Name);
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, Stat, true, FVector2D(1.5, 1.5));
}

void ABaseGeometryActor::PrintTransform()
{
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();


	UE_LOG(LogBaseGeometry, Warning, TEXT("Location  : %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotator  : %s"), *Rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale  : %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Scale  : %s"), *Transform.ToHumanReadableString());
	PrintStringTypes();
}

void ABaseGeometryActor::HandleMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
	{
		FVector CurrentLocation = GetActorLocation();

		float Time = GetWorld()->GetTimeSeconds();
		CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		SetActorLocation(CurrentLocation);
	}
	break;

	case EMovementType::Static: break;

	default: break;
	
	}

	  
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);

	}
}

void ABaseGeometryActor::OnTimerFierd()
{
	if (++TimerCount< MaxTimmerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseGeometry, Display, TEXT("TimerCount: %i Color to set up  : %s"), TimerCount,*NewColor.ToString());
		SetColor(FLinearColor::MakeRandomColor());
	}
	else
	{
		UE_LOG(LogBaseGeometry, Display, TEXT("Timer has been stopped!"));

		GetWorldTimerManager().ClearTimer(TimerHandle);
	}

}

