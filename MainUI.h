// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	class ATPSPlayController *PlayerController;

	class AHero *HeroCharacter;

	class ARiffle *Riffle;

	class ALauncher *Launcher;



	int32 BulletInAmmo_Riffle;

	int32 CurrenBullet_Riffle;

	int32 MaxBullet_Riffle;

	int32 BulletInAmmo_Launcher;

	int32 CurrenBullet_Launcher;

	int32 MaxBullet_Launcher;

	int32 CurrentHP;

	int32 MaxHp;

	float TickTime;

	FVector2D BasicAimSize;


public:
	UFUNCTION()
		void GettingBulletInfo();
	UFUNCTION()
		void SettingBulletText();
	UFUNCTION()
		void GettingHP();
	UFUNCTION()
		void SettingHP();
protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess=true))
		class UTextBlock *Text_CurrentBullet_Riffle;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UTextBlock *Text_BulletInAmmo_Riffle;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UTextBlock *Text_CurrentBullet_Launcher;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UTextBlock *Text_BulletInAmmo_Launcher;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UTextBlock *Text_CurrentHP;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UProgressBar *HPBar;

	UPROPERTY(Meta = (BindWidget), Meta=(AllowPrivateAccess = true))
		class UImage *AimImage;

};
