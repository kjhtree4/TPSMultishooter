// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "LauncherAmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API ALauncherAmmoItem : public AItem
{
	GENERATED_BODY()
	
//
public:

	ALauncherAmmoItem();

	virtual void ItemFunction(class AHero *OverlappedActor) override;

	virtual	bool CanBeEaten(class AHero *Hero) override;

private:

	int32 AddBullet;
};
