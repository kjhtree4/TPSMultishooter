// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthItem.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API AHealthItem : public AItem
{
	GENERATED_BODY()
	
public:
	AHealthItem();

	virtual void ItemFunction(class AHero *OverlappedActor) override;

	virtual	bool CanBeEaten(class AHero *Hero) override;
private:
	int32 AddHP;
};
