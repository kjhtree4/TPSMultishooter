// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "LauncherAmmoItem.h"

ALauncherAmmoItem::ALauncherAmmoItem()
{
	
	AddBullet = 2;
	RespawnTime = 5.f;
	ItemGetString = FString(TEXT("Get Launcher Ammo"));

	static ConstructorHelpers::FObjectFinder<USoundBase> S(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_AmmoPickup_Cue.AssaultRifle_AmmoPickup_Cue'"));
	if (S.Succeeded())
	{
		Sound = S.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Gameplay/Pick-ups/P_Launcher_Ammo.P_Launcher_Ammo'"));
	if (P.Succeeded())
	{
		ParticleSystem->SetTemplate(P.Object);
	}
	bReplicates = true;
}


void ALauncherAmmoItem::ItemFunction(AHero * OverlappedActor)
{
	OverlappedActor->AddLauncherBullet_Client(AddBullet);
}

bool ALauncherAmmoItem::CanBeEaten(AHero * Hero)
{
	return true;
}
