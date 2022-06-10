// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSGameMode.h"
#include "Net/UnrealNetwork.h"
#include "TPSGameState.h"

ATPSGameState::ATPSGameState()
{
	MainTime = 100;
}

void ATPSGameState::LogoutAllControllers()
{
	if (UGameplayStatics::GetGameMode(GetWorld()))
	{
		ATPSGameMode *TPSGameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		TPSGameMode->AllControllerLogOut();
	}
}

void ATPSGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		MainTimeReduce = MainTime;
	}
}

void ATPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameState, FalseTeamScore);
	DOREPLIFETIME(ATPSGameState, TrueTeamScore);
	DOREPLIFETIME(ATPSGameState, MainTimeReduce);
	DOREPLIFETIME(ATPSGameState, WinnerText);
}

void ATPSGameState::MainTimerEvent()
{
	MainTimeReduce = MainTimeReduce - 1;
	if (MainTimeReduce <= 0)
	{
		FTimerManager &TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(MainTimerHandler);

		GameEnd_Server();
	}
}

void ATPSGameState::GameEnd_Server_Implementation()
{
	FTimerManager &TimerManager = GetWorld()->GetTimerManager();

	if (FalseTeamScore < TrueTeamScore)
	{
		WinnerText = FString(TEXT("TrueTeamWin"));
	}
	else if(FalseTeamScore == TrueTeamScore)
	{
		WinnerText = FString(TEXT("Draw"));
	}
	else if (FalseTeamScore > TrueTeamScore)
	{
		WinnerText = FString(TEXT("FalseTeamWin"));
	}
	TimerManager.SetTimer(LogoutAllControllersHandler, this, &ATPSGameState::LogoutAllControllers, 10.0f, false);
}

void ATPSGameState::MainTimer_Server_Implementation()
{
	FTimerManager &TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(MainTimerHandler, this, &ATPSGameState::MainTimerEvent, 1.0f, true);
}


void ATPSGameState::OnRep_FalseTeamScore()
{
	OnScoreChangeUI.Broadcast();
}

void ATPSGameState::OnRep_TrueTeamScore()
{
	OnScoreChangeUI.Broadcast();
}

void ATPSGameState::OnRep_MainTimeReduce()
{
	OnMainTimerChangeForUI.Broadcast();
}

void ATPSGameState::OnRep_WinnerText()
{
	OnWinTextChangeForUI.Broadcast();
}
