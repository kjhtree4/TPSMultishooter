// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "RiffleAmmoItem.h"
//
ARiffleAmmoItem::ARiffleAmmoItem()
{
	
	AddBullet = 50;
	RespawnTime = 5.f;
	ItemGetString = FString(TEXT("Get Riffle Ammo"));

	static ConstructorHelpers::FObjectFinder<USoundBase> S(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_AmmoPickup_Cue.AssaultRifle_AmmoPickup_Cue'"));
	if (S.Succeeded())
	{
		Sound = S.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Gameplay/Pick-ups/P_AssaultRifle_AMmo.P_AssaultRifle_AMmo'"));
	if (P.Succeeded())
	{
		ParticleSystem->SetTemplate(P.Object);
	}
	bReplicates = true;
}


void ARiffleAmmoItem::ItemFunction(AHero * OverlappedActor)
{
	OverlappedActor->AddRiffleBullet_Client(AddBullet);
}

bool ARiffleAmmoItem::CanBeEaten(AHero * Hero)
{
	return true;
}
