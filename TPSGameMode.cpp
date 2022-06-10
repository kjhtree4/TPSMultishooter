// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "HeroPlayController.h"
#include "Kismet/KismetArrayLibrary.h"
#include "TPSGameState.h"
#include "TPSGameMode.h"

ATPSGameMode::ATPSGameMode()
{
	MaxPlayer = 4;
	TrueTeamScore = 0;
	FalseTeamScore = 0;
	CountToJoinBattleField = 10;

	GameStateClass = ATPSGameState::StaticClass();
	PlayerControllerClass = AHeroPlayController::StaticClass();
	
	bUseSeamlessTravel = true;
	bStartPlayersAsSpectators = true;
}

void ATPSGameMode::BeginPlay()
{
	FTimerManager &TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(CheckingLoginPCTimer, this, &ATPSGameMode::CheckingLoginPC, 1.f, true);

	if (UGameplayStatics::GetGameState(GetWorld()))
	{
		TPSGameState = Cast<ATPSGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}


void ATPSGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	if (Cast<AHeroPlayController>(Exiting))
	{
		AHeroPlayController *LogoutPlayer = Cast<AHeroPlayController>(Exiting);
		int AliveControllerNum = JoinPlayerControllers.Num();
		for (int i =0 ; i<AliveControllerNum; i++)
		{
			if (LogoutPlayer == JoinPlayerControllers[i])
			{
				JoinPlayerControllers.Remove(LogoutPlayer);
			}
		}
	}
}

void ATPSGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (Cast<AHeroPlayController>(NewPlayer))
	{
		AHeroPlayController *LoginPlayer = Cast<AHeroPlayController>(NewPlayer);

		if (JoinPlayerControllers.Num() < MaxPlayer)
		{
			JoinPlayerControllers.Add(LoginPlayer);
		}
		else 
		{
			NewPlayer->SetLifeSpan(0.1f);
		}
	}
	else
	{
		NewPlayer->SetLifeSpan(0.1f);
	}

}

void ATPSGameMode::CheckingLoginPC()
{
	if (JoinPlayerControllers.Num() == MaxPlayer)
	{
		SayToAll(FString::FromInt(CountToJoinBattleField));
		if (CountToJoinBattleField <= 0)
		{
			GetSpawnPoint();
			FTimerManager &TimerManager = GetWorld()->GetTimerManager();
			TimerManager.ClearTimer(CheckingLoginPCTimer);
			SayToAll(FString(TEXT("")));
		}
		else
		{
			CountToJoinBattleField = CountToJoinBattleField - 1;
		}
	}
	else
	{
		SayToAll(FString(TEXT("WaitingForPlayers...")));
		CountToJoinBattleField = 10;
	}
}

void ATPSGameMode::GetSpawnPoint()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), GetAllActorOfClassArray);
	int SpawnPointNum = GetAllActorOfClassArray.Num();
	

	for (int i =0; i<SpawnPointNum ; i++)
	{
		SpawnPointArray.Add(Cast<ASpawnPoint>(GetAllActorOfClassArray[i]));
	}

	SpawnPointNum = SpawnPointArray.Num();

	for (int j=0 ; j<SpawnPointNum ; j++)
	{
		if (SpawnPointArray[j]->TeamDivide)
		{
			TeamTrueSpawnPoint.Add(SpawnPointArray[j]->GetActorLocation());
			TeamTrueSpawnRotate.Add(SpawnPointArray[j]->GetActorRotation());
		}
		else
		{
			TeamFalseSpawnPoint.Add(SpawnPointArray[j]->GetActorLocation());
			TeamFalseSpawnRotate.Add(SpawnPointArray[j]->GetActorRotation());
		}
	}
	SetTeam();
}

void ATPSGameMode::SetTeam()
{
	for (int i=0; i<JoinPlayerControllers.Num(); i++)
	{
		if (i < (MaxPlayer / 2))
		{
			JoinPlayerControllers[i]->Team = false;
			JoinPlayerControllers[i]->SpawnPoint = TeamFalseSpawnPoint[i];
			JoinPlayerControllers[i]->SpawnRotate = TeamFalseSpawnRotate[i];
		}
		else
		{
			JoinPlayerControllers[i]->Team = true;;
			JoinPlayerControllers[i]->SpawnPoint = TeamTrueSpawnPoint[i-(MaxPlayer/2)];
			JoinPlayerControllers[i]->SpawnRotate = TeamTrueSpawnRotate[i-(MaxPlayer / 2)];
		}
	}
	SpawnHero();
}

void ATPSGameMode::Scoring(bool Team)
{
	if (Team)
	{
		FalseTeamScore = FalseTeamScore + 1;
		TPSGameState->FalseTeamScore = (TPSGameState->FalseTeamScore) + 1;
	}
	else
	{
		TrueTeamScore = TrueTeamScore + 1;
		TPSGameState->TrueTeamScore = (TPSGameState->TrueTeamScore) + 1;
	}
}

void ATPSGameMode::AllControllerLogOut()
{
	int AliveControllerNum = JoinPlayerControllers.Num();

	for (int i =0 ; i<AliveControllerNum ; i++)
	{
		JoinPlayerControllers[i]->SetLifeSpan(0.1f);
	}
}

void ATPSGameMode::SayToAll(FString Notification)
{
	int AliveControllerNum = JoinPlayerControllers.Num();

	for (int i = 0; i < AliveControllerNum; i++)
	{
		JoinPlayerControllers[i]->NotificationFromServer = Notification;
	}
}

void ATPSGameMode::SpawnHero()
{
	int AliveControllerNum = JoinPlayerControllers.Num();

	for (int i = 0; i < AliveControllerNum; i++)
	{
		JoinPlayerControllers[i]->Spawn();
	}
}