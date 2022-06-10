// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Kismet/KismetMathLibrary.h"
#include "Riffle.h"

ARiffle::ARiffle()
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));
	if (SK.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SK.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/ParagonDrongo/FX/Particles/Abilities/Primary/FX/P_Drongo_Primary_MuzzleFlash.P_Drongo_Primary_MuzzleFlash'"));
	if (Fire.Succeeded())
	{
		FireEmitter = Fire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HeroHit(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Gameplay/Player/P_body_bullet_impact.P_body_bullet_impact'"));
	if (HeroHit.Succeeded())
	{
		HeroHitEmitter = HeroHit.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ObjectHit(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (ObjectHit.Succeeded())
	{
		ObjectHitEmitter = ObjectHit.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FireS(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_Cue.AssaultRifle_Cue'"));
	if (FireS.Succeeded())
	{
		FireSound = FireS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> HitS(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRiffle_HitCue.AssaultRiffle_HitCue'"));
	if (HitS.Succeeded())
	{
		HitSound = HitS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> AmmoS(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/WeaponAmmoEmpty_Cue.WeaponAmmoEmpty_Cue'"));
	if (AmmoS.Succeeded())
	{
		NoAmmoSound = AmmoS.Object;
	}

	Damage = 20;
	MaxBullet = 30;
	CurrentBullet = 0;
	BulletInAmmo = 0;
	FireTerm = 0.1f;
	BulletDistance = 100000.f;
	AimSpread = 0.f;
}

void ARiffle::FireEvent()
{
	EPlayerStatus PlayerStatus = OwnerCharacter->PlayerStatus;

	if (PlayerStatus == EPlayerStatus::Reloading || PlayerStatus == EPlayerStatus::Die || PlayerStatus == EPlayerStatus::Swapping)
	{
		FTimerManager &TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(FireEventHandle, this, &ARiffle::FireEvent, FireTerm, true);
	}
	else 
	{
		if (CheckBulletWhenShooting())
		{
			CurrentBullet = CurrentBullet - 1;
			OnRiffleFireDelegate.Broadcast();

			float SpreadedPitch = UKismetMathLibrary::RandomFloatInRange(AimSpread*(-1), AimSpread) * 2;
			float SpreadedYaw = UKismetMathLibrary::RandomFloatInRange(AimSpread*(-1), AimSpread) * 2;
			float SpreadedRoll = UKismetMathLibrary::RandomFloatInRange(AimSpread*(-1), AimSpread) * 2;

			FRotator SpreadedRot = UKismetMathLibrary::ComposeRotators(OwnerCharacter->GetControlRotation(), UKismetMathLibrary::MakeRotator(SpreadedRoll, SpreadedPitch, SpreadedYaw));
			
			FVector Start = OwnerCharacter->SpringArm->GetComponentLocation();
			FVector End = (UKismetMathLibrary::GetForwardVector(SpreadedRot)*BulletDistance) + Start;
			TArray <AActor*> ActorToIgnore; 
			ActorToIgnore.Add(OwnerCharacter);
			FHitResult OutHit;

			bool TraceValid = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypeForLinetrace, false, ActorToIgnore, EDrawDebugTrace::Persistent, OutHit, true);
			
			
			AimSpreadFunc();

			if (TraceValid)
			{
				OwnerCharacter->BulletHitProcess_Server(OutHit.ImpactPoint + (OutHit.ImpactNormal*5.f), OutHit.GetActor(), 1);
			}
			else
			{
				OwnerCharacter->RiffleFireEmitter_Server();
			}

			OwnerCharacter->RiffleShootCameraShake_Client();
		}
		else
		{
			FTimerManager &TimerManager = GetWorld()->GetTimerManager();
			TimerManager.ClearTimer(FireEventHandle);
		}

	}
}