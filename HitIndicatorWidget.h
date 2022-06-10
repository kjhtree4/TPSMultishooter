// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitIndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API UHitIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void Animation();
	    virtual	void Animation_Implementation();

	UPROPERTY(Meta = (BindWidgetAnim), BlueprintReadWrite)
		class UWidgetAnimation *StartAnim;
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation *EndAnim;

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
private:



	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true), BlueprintReadWrite, EditAnywhere)
		class UBorder *IndicatorBorder;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		class UMaterialInstanceDynamic *Material;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		FVector HitLocation;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		class AHero *HeroCharacter;


};
