// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/Actor.h"
#include "LauncherBullet.generated.h"

UCLASS()
class TPSMULTISHOOTER_API ALauncherBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALauncherBullet();

	class AHero *Ownercharacter;
	class ALauncher *Launcher;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USphereComponent *Sphere;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent *ParticleSystemComponent;
	UPROPERTY(EditAnywhere)
		UArrowComponent *Arrow;
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent *ProjectileMovementComponent;

public:	
	UFUNCTION()
		void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	

};
