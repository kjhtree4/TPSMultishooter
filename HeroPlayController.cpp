// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "TrueTeamHero.h"
#include "FasleTeamHero.h"
#include "TPSGameMode.h"
#include "TPSGameState.h"
#include "Net/UnrealNetwork.h"
#include "Riffle.h"
#include "Launcher.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Math/Quat.h"
#include "HeroPlayController.h"


AHeroPlayController::AHeroPlayController()
{
	
	static ConstructorHelpers::FClassFinder<UUserWidget> Score(TEXT("WidgetBlueprint'/Game/Widgets/ScoreUI.ScoreUI_C'"));
	if (Score.Succeeded())
	{
		ScoreUIClass = Score.Class;
	}
	

	static ConstructorHelpers::FObjectFinder<USoundBase> GameMusic(TEXT("SoundCue'/Game/Sounds/Music/ShooterGame_Music_Cue.ShooterGame_Music_Cue'"));
	if (GameMusic.Succeeded())
	{
		TPSMultiShooterMusic = GameMusic.Object;
	}

	RespawnTime = 10.f;
}


void AHeroPlayController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayController, Hero);
	DOREPLIFETIME(AHeroPlayController, NotificationFromServer);

}


void AHeroPlayController::RespawnTimerExecuter_Server_Implementation()
{
	RespawnTimeReduce = RespawnTime;

	FTimerManager &TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(SpawnTimerHandle, this, &AHeroPlayController::Spawn, RespawnTime, false);

	if (IsValid(Hero))
	{
		TPSGameMode->Scoring(Team);
		TimerManager.SetTimer(RespawnTimeCounterForUIHandle, this, &AHeroPlayController::RespawnTimeCounterForUI, 1.f, true);
	}
}

void AHeroPlayController::SetHeroWhenSpawn_Server_Implementation()
{
	if (Cast<AHero>(GetPawn()))
	{
		Hero = Cast<AHero>(GetPawn());
	}
}

void AHeroPlayController::OnRep_NotificationFromServer()
{
	OnNotificationChanged.Broadcast();
}

void AHeroPlayController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		if (UGameplayStatics::GetGameMode(GetWorld())) 
		{
			TPSGameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (UGameplayStatics::GetGameState(GetWorld()))
			{
				TPSGameState = Cast<ATPSGameState>(UGameplayStatics::GetGameState(GetWorld()));
			}
		}
	}
	else 
	{
		ScoreUI = CreateWidget(this, ScoreUIClass);
		if (ScoreUI != nullptr) {
			ScoreUI->AddToViewport();

		}
		UGameplayStatics::PlaySound2D(GetWorld(), TPSMultiShooterMusic);
	}
}

void AHeroPlayController::SetControllerRotaion_Client_Implementation(FRotator Rotator)
{
	this->SetControlRotation(Rotator);
}

void AHeroPlayController::SayToClient(FString Notification)
{
	NotificationFromServer = Notification;
}

void AHeroPlayController::RespawnTimeCounterForUI()
{
	RespawnTimeReduce = RespawnTimeReduce - 1;
	if (RespawnTimeReduce <= 0.f)
	{
		SayToClient(FString(TEXT(" ")));
		FTimerManager &TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(RespawnTimeCounterForUIHandle);
	}
	else {
		FString RespawnT = FString::FromInt(RespawnTimeReduce);
		RespawnT.Append(FString(TEXT("...")));
		FString RespawnText = FString(TEXT("Respawn In  "));
		RespawnText.Append(RespawnT);
		
		SayToClient(RespawnText);
	}
}

void AHeroPlayController::Spawn()
{
	
	if (IsValid(TPSGameMode))
	{
		if (IsValid(Hero))
		{
			Hero->RemoveMainUI_Client();
			Hero->RemoveLowHPWidget_Client();
			DestroyActor_Server();
			SetControllerRotaion_Client(SpawnRotate);
			
			FTransform HeroSpawnTransform;
			HeroSpawnTransform.SetLocation(SpawnPoint);
			HeroSpawnTransform.SetRotation(FQuat(SpawnRotate));

			AHero *SpawnedHero;
			if (Team)
			{
				SpawnedHero = GetWorld()->SpawnActor<AHero>(ATrueTeamHero::StaticClass(), HeroSpawnTransform);
			}
			else
			{
				SpawnedHero = GetWorld()->SpawnActor<AHero>(AFasleTeamHero::StaticClass(), HeroSpawnTransform);
			}

			
			SpawnedHero->Team = this->Team;
			this->Possess(SpawnedHero);
			SetHeroWhenSpawn_Server();
			SpawnedHero->ChangeColByTeam_Server(Team);
			
		}
		else
		{
			if (IsValid(GetPawn()))
				GetPawn()->Destroy();

			SetControllerRotaion_Client(SpawnRotate);

			FTransform HeroSpawnTransform;
			HeroSpawnTransform.SetLocation(SpawnPoint);
			HeroSpawnTransform.SetRotation(FQuat(SpawnRotate));

			AHero *SpawnedHero;
			if (Team)
			{
				SpawnedHero = GetWorld()->SpawnActor<AHero>(ATrueTeamHero::StaticClass(), HeroSpawnTransform);
			}
			else
			{
				SpawnedHero = GetWorld()->SpawnActor<AHero>(AFasleTeamHero::StaticClass(), HeroSpawnTransform);
			}
			SpawnedHero->Team = this->Team;
			this->Possess(SpawnedHero);
			SetHeroWhenSpawn_Server();
			TPSGameState->MainTimer_Server();

			SpawnedHero->ChangeColByTeam_Server(Team);
		}
	}
}

void AHeroPlayController::DestroyWeapon_Client_Implementation()
{
	Hero->Riffle->Destroy();
	Hero->Launcher->Destroy();
	Hero->Destroy();
}

void AHeroPlayController::DestroyActor_Server_Implementation()
{
	DestroyWeapon_Client();
	Hero->Riffle->Destroy();
	Hero->Launcher->Destroy();
	Hero->Destroy();
}
