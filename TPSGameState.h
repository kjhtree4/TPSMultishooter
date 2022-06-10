// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnScoreChangeUI);
DECLARE_MULTICAST_DELEGATE(FOnMainTimerChangeForUI);
DECLARE_MULTICAST_DELEGATE(FOnWinTextChangeForUI);

UCLASS()
class TPSMULTISHOOTER_API ATPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATPSGameState();

	UPROPERTY(ReplicatedUsing = OnRep_FalseTeamScore)
		int32 FalseTeamScore;
	UFUNCTION()
		void OnRep_FalseTeamScore();

	UPROPERTY(ReplicatedUsing = OnRep_TrueTeamScore)
		int32 TrueTeamScore;
	UFUNCTION()
		void OnRep_TrueTeamScore();

	int32 MainTime;

	UPROPERTY(ReplicatedUsing = OnRep_MainTimeReduce)
		int32 MainTimeReduce;
	UFUNCTION()
		void OnRep_MainTimeReduce();
	UPROPERTY(ReplicatedUsing = OnRep_WinnerText)
		FString WinnerText;
	UFUNCTION()
		void OnRep_WinnerText();

	FTimerHandle MainTimerHandler;
	FTimerHandle LogoutAllControllersHandler;

	FOnScoreChangeUI OnScoreChangeUI;
	FOnMainTimerChangeForUI OnMainTimerChangeForUI;
	FOnWinTextChangeForUI OnWinTextChangeForUI;

public:
	
	UFUNCTION(Server, Reliable)
		void MainTimer_Server();

	UFUNCTION()
		void MainTimerEvent();
	
	UFUNCTION(Server, Reliable)
		void GameEnd_Server();

	UFUNCTION()
		void LogoutAllControllers();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
