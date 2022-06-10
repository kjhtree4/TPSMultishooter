// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class TPSMULTISHOOTER_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	UPROPERTY(EditAnywhere)
		USceneComponent *SpawnPoint;

	UPROPERTY(EditAnywhere)
		class UArrowComponent *Arrow;

	ASpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TeamDivide;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
