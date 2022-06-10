// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Riffle.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnRiffleFireDelegate);
/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API ARiffle : public AWeapon
{
	GENERATED_BODY()
	
public:

	FOnRiffleFireDelegate OnRiffleFireDelegate;

	ARiffle();

	virtual void FireEvent() override;
};
