// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TPSMULTISHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere)
		USphereComponent *SphereCol;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent *ParticleSystem;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *Floor;

	class AHeroPlayController *Controller;

	float RespawnTime;

	UPROPERTY(EditAnywhere)
		class USoundBase * Sound;

	UPROPERTY(ReplicatedUsing = OnRep_IsActive)
		bool IsActive;

	UFUNCTION()
		void OnRep_IsActive();

	FString ItemGetString;

	UFUNCTION()
		virtual	bool CanBeEaten(class AHero *Hero);

protected:
	FTimerHandle NotificationClearHandle;
	FTimerHandle RespawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void Respawn();

	UFUNCTION()
		void NotificationClear();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		virtual void ItemFunction(class AHero *OverlappedActor);
	

	
};
