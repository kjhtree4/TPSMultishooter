// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayController.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnNotificationChanged)

UCLASS()
class TPSMULTISHOOTER_API AHeroPlayController : public APlayerController
{
	GENERATED_BODY()
	

public:

	AHeroPlayController();

	UPROPERTY(ReplicatedUsing = OnRep_NotificationFromServer)
		FString NotificationFromServer;

	UFUNCTION()
		void OnRep_NotificationFromServer();

	bool Team;
	FVector SpawnPoint;
	FRotator SpawnRotate;

protected:

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UUserWidget> ScoreUIClass;

	UPROPERTY(VisibleAnywhere)
		UUserWidget *ScoreUI;

	class ATPSGameMode *TPSGameMode;
	class ATPSGameState *TPSGameState;

	UPROPERTY(Replicated)
		class AHero *Hero;

	UPROPERTY(VisibleAnywhere)
		class USoundBase *TPSMultiShooterMusic;

	float RespawnTime;
	float RespawnTimeReduce;

	FTimerHandle RespawnTimeCounterForUIHandle;
	FTimerHandle SpawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:

	UFUNCTION(Client, Reliable)
		void SetControllerRotaion_Client(FRotator Rotator);

	UFUNCTION()
		void SayToClient(FString Notification);

	FOnNotificationChanged OnNotificationChanged;

	UFUNCTION(Server, Reliable)
		void RespawnTimerExecuter_Server();

	UFUNCTION()
		void RespawnTimeCounterForUI();

	UFUNCTION()
		void Spawn();

	UFUNCTION(Server, Reliable)
		void SetHeroWhenSpawn_Server();

	UFUNCTION(Server, Reliable)
		void DestroyActor_Server();

	UFUNCTION(Client, Reliable)
		void DestroyWeapon_Client();
};
