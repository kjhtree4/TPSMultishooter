// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TPSMULTISHOOTER_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATPSGameMode();

protected:
	UPROPERTY()
		TArray<AActor *>GetAllActorOfClassArray;
	UPROPERTY()
		TArray<class ASpawnPoint *> SpawnPointArray;
	UPROPERTY()
		TArray<class AHeroPlayController *> JoinPlayerControllers;
	UPROPERTY()
		TArray<FVector> TeamTrueSpawnPoint;
	UPROPERTY()
		TArray<FVector> TeamFalseSpawnPoint;
	UPROPERTY()
		TArray<FRotator> TeamTrueSpawnRotate;
	UPROPERTY()
		TArray<FRotator> TeamFalseSpawnRotate;

	int32 MaxPlayer;
	int32 TrueTeamScore;
	int32 FalseTeamScore;
	int32 CountToJoinBattleField;

	FTimerHandle CheckingLoginPCTimer;

	class ATPSGameState *TPSGameState;

public:

	void SpawnHero();

	void Scoring(bool Team);
	
	void AllControllerLogOut();

	void SayToAll(FString Notification);

protected:
	

	virtual void BeginPlay() override;

	virtual void Logout(AController* Exiting)override;

	virtual void PostLogin(APlayerController* NewPlayer)override;

	UFUNCTION()	
		void CheckingLoginPC();

		void GetSpawnPoint();

		void SetTeam();

	

};
