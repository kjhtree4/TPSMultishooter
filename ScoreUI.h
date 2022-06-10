// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "Blueprint/UserWidget.h"
#include "ScoreUI.generated.h"

/**
 * 
 */


UCLASS()
class TPSMULTISHOOTER_API UScoreUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void SetNotificationFromServer();
	UFUNCTION()
		void ChangeScore();
	UFUNCTION()
		void ChangeTime();
	UFUNCTION()
		void SetWinText();



protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class ATPSGameState *TPSGameState;
	UPROPERTY()
		class AHeroPlayController *HeroController;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
		class UTextBlock *FalseTeamScore;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
		class UTextBlock *TrueTeamScore;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
		class UTextBlock *Timer;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
		class UTextBlock *NotificationFromServerText;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
		class UTextBlock *WinText;
};
