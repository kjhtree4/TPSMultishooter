// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Weapon.generated.h"



UCLASS()
class TPSMULTISHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent *SkeletalMesh;
	UPROPERTY(EditAnywhere)
		USceneComponent *FireMuzzle;

public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(Replicated)
		int32 MaxBullet;

	int32 CurrentBullet;
	int32 BulletInAmmo;
	int32 Damage;

	float FireTerm;
	float BulletDistance;
	float AimSpread;

	UPROPERTY(Replicated)
		FVector ImpactPoint;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeForLinetrace;

	UPROPERTY()
		class AHero *OwnerCharacter;

	UPROPERTY()
		class UMainUI *OwnersHUD;

	UPROPERTY(EditAnywhere)
		UParticleSystem *HeroHitEmitter;
	UPROPERTY(EditAnywhere)
		UParticleSystem *FireEmitter;
	UPROPERTY(EditAnywhere)
		UParticleSystem *ObjectHitEmitter;
	
	UPROPERTY(EditAnywhere)
		USoundBase *FireSound;
	UPROPERTY(EditAnywhere)
		USoundBase *HitSound;
	UPROPERTY(EditAnywhere)
		USoundBase *NoAmmoSound;

	FTimerHandle FireEventHandle;


private:
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	


	void AttachToOwnerHand();
	void AttachToOwnerBack();

	
	void Fire();

	UFUNCTION()
		virtual void FireEvent();

	UFUNCTION(Client, UnReliable)
		void AimSpreadFunc();


	UFUNCTION(NetMulticast, UnReliable)
		void SpawnFireEmitter();
	UFUNCTION(NetMulticast, UnReliable)
		void SpawnFireSound();
	UFUNCTION(NetMulticast, UnReliable)
		void SpawnObjectHitEmitter();
	UFUNCTION(NetMulticast, UnReliable)
		void SpawnObjectHitSound();
	UFUNCTION(NetMulticast, UnReliable)
		void SpawnHeroHitEmitter();


	void Reload();

	void NoAmmo();

	bool CheckBulletWhenShooting();
	
};
