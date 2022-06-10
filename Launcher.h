// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Launcher.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnLaucherFireDelegate);

UCLASS()
class TPSMULTISHOOTER_API ALauncher : public AWeapon
{
	GENERATED_BODY()
	

public:

	//int32 temp;

	FOnLaucherFireDelegate OnLauncherFireDelegate;

	ALauncher();

	virtual void FireEvent() override;

	bool LauncherCoolTimeDone;
	FRotator BulletSpawnRotation;
	float DamageRadius;

private:

	FTimerHandle FLauncherCoolTimeHandle;

	UFUNCTION()
		void LauncherCoolTime();
};
