// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "RiffleAmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API ARiffleAmmoItem : public AItem
{
	GENERATED_BODY()
	
public:

	ARiffleAmmoItem();

	virtual void ItemFunction(class AHero *OverlappedActor) override;

	virtual	bool CanBeEaten(class AHero *Hero) override;

private:
	int32 AddBullet;
};
