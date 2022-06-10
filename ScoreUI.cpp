// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "TPSGameState.h"
#include "Components/TextBlock.h"
#include "HeroPlayController.h"
#include "ScoreUI.h"

void UScoreUI::NativeConstruct()
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		HeroController = Cast<AHeroPlayController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		HeroController->OnNotificationChanged.AddUObject(this, &UScoreUI::SetNotificationFromServer);

		if (UGameplayStatics::GetGameState(GetWorld()))
		{
			TPSGameState = Cast<ATPSGameState>(UGameplayStatics::GetGameState(GetWorld()));
			TPSGameState->OnScoreChangeUI.AddUObject(this, &UScoreUI::ChangeScore);
			TPSGameState->OnMainTimerChangeForUI.AddUObject(this, &UScoreUI::ChangeTime);
			TPSGameState->OnWinTextChangeForUI.AddUObject(this, &UScoreUI::SetWinText);
		}
	}
}


void UScoreUI::ChangeScore()
{
	TrueTeamScore->SetText(FText::AsNumber(TPSGameState->TrueTeamScore));
	FalseTeamScore->SetText(FText::AsNumber(TPSGameState->FalseTeamScore));

}

void UScoreUI::ChangeTime()
{
	Timer->SetText(FText::AsNumber(TPSGameState->MainTimeReduce));
}

void UScoreUI::SetWinText()
{
	WinText->SetText(FText::FromString(TPSGameState->WinnerText)); 
}

void UScoreUI::SetNotificationFromServer()
{
	NotificationFromServerText->SetText(FText::FromString(HeroController->NotificationFromServer));
}



