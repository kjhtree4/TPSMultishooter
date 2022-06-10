 //Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "HealthItem.h"

AHealthItem::AHealthItem()
{
	
	AddHP = 50;
	RespawnTime = 5.f;
	ItemGetString = FString(TEXT("Get Health"));

	
	static ConstructorHelpers::FObjectFinder<USoundBase> S(TEXT("SoundCue'/Game/Sounds/Stingers/Mono/StingerPickup_Health_Cue.StingerPickup_Health_Cue'"));
	if (S.Succeeded())
	{
		Sound = S.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Gameplay/Pick-ups/P_Health.P_Health'"));
	if (P.Succeeded())
	{
		ParticleSystem->SetTemplate(P.Object);
	}
	bReplicates = true;
}


void AHealthItem::ItemFunction(class AHero *OverlappedActor)
{
	OverlappedActor->AddHP_Server(AddHP);
}


bool AHealthItem::CanBeEaten(class AHero * Hero)
{
	if (Hero->CurrentHP < Hero->MaxHP)
	{
		return true;;
	}
	else 
	{
		return false;
	}

	return false;
}
