// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ArrowComponent.h"
#include "SpawnPoint.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	RootComponent = SpawnPoint;
	Arrow->SetupAttachment(SpawnPoint);



	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

