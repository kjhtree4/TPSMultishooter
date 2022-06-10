// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "Blueprint/UserWidget.h"
#include "LowHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API ULowHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULowHPWidget(const FObjectInitializer &ObjectInitializer);

	UFUNCTION()
		void AnimationStop();

	UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
		class UWidgetAnimation *FadeIn;
	UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
		class UWidgetAnimation *FadeOut;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	UPROPERTY()
		class UAudioComponent *LowHpSoundComponent;

	UPROPERTY(EditAnywhere ,Meta = (AllowPrivateAccess = true))
		class USoundBase *LowHpSound;
	
};
