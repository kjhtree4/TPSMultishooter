// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	//virtual void NativeUpdateAnimation(float DeltaSecond) override;
	virtual void NativeUpdateAnimation(float DeltaSecond) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool UseControllerRotationYaw;

protected:
	class AHero *Hero;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AimPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsInAir;
	

private:
	UFUNCTION()
		void AnimNotify_ReloadEnd();
	UFUNCTION()
		void AnimNotify_SwapEnd();

};
