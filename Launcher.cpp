// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Kismet/KismetMathLibrary.h"
#include "LauncherBullet.h"
#include "Math/Quat.h"
#include "Launcher.h"

ALauncher::ALauncher()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK(TEXT("SkeletalMesh'/Game/Weapons/Launcher.Launcher'"));
	if (SK.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SK.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/RocketLauncher/Muzzle/P_Launcher_MF.P_Launcher_MF'"));
	if (Fire.Succeeded())
	{
		FireEmitter = Fire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HeroHit(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/RocketLauncher/Impact/P_Launcher_IH.P_Launcher_IH'"));
	if (HeroHit.Succeeded())
	{
		HeroHitEmitter = HeroHit.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ObjectHit(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/RocketLauncher/Impact/P_Launcher_IH.P_Launcher_IH'"));
	if (ObjectHit.Succeeded())
	{
		ObjectHitEmitter = ObjectHit.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FireS(TEXT("SoundCue'/Game/Sounds/Weapon_GrenadeLauncher/Mono/GrenadeLauncher_Shot_Cue.GrenadeLauncher_Shot_Cue'"));
	if (FireS.Succeeded())
	{
		FireSound = FireS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> HitS(TEXT("SoundCue'/Game/Sounds/Weapon_GrenadeLauncher/Mono/GrenadeLauncher_Explosion_Cue.GrenadeLauncher_Explosion_Cue'"));
	if (FireS.Succeeded())
	{
		HitSound = HitS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> AmmoS(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/WeaponAmmoEmpty_Cue.WeaponAmmoEmpty_Cue'"));
	if (AmmoS.Succeeded())
	{
		NoAmmoSound = AmmoS.Object;
	}


	Damage = 80;
	MaxBullet = 2;
	CurrentBullet = 0;
	BulletInAmmo = 0;
	FireTerm = 1.5f;
	BulletDistance = 100000.f;
	AimSpread = 0.f;
	DamageRadius = 300.f;
	
	LauncherCoolTimeDone = true;
}

void ALauncher::FireEvent()
{
	EPlayerStatus PlayerStatus = OwnerCharacter->PlayerStatus;
	FTimerManager &TimerManager = GetWorld()->GetTimerManager();

	if (PlayerStatus == EPlayerStatus::Reloading || PlayerStatus == EPlayerStatus::Die || PlayerStatus == EPlayerStatus::Swapping)
	{
		TimerManager.SetTimer(FireEventHandle, this, &ALauncher::FireEvent, FireTerm, true);
	}
	else
	{
		if (LauncherCoolTimeDone) 
		{
			if (CheckBulletWhenShooting())
			{
				LauncherCoolTimeDone = false;
				TimerManager.SetTimer(FLauncherCoolTimeHandle, this, &ALauncher::LauncherCoolTime, FireTerm, false);

				CurrentBullet = CurrentBullet - 1;
				OnLauncherFireDelegate.Broadcast();

				FVector Start = OwnerCharacter->SpringArm->GetComponentLocation();
				FVector End = (UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation()))*BulletDistance + Start;
				TArray <AActor*> ActorToIgnore;
				FHitResult OutHit;

				bool TraceValid = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypeForLinetrace, false, ActorToIgnore, EDrawDebugTrace::None, OutHit, true);

				if (TraceValid)
					BulletSpawnRotation = UKismetMathLibrary::FindLookAtRotation(FireMuzzle->GetComponentLocation(), OutHit.Location);
				else
					BulletSpawnRotation = UKismetMathLibrary::FindLookAtRotation(FireMuzzle->GetComponentLocation(), OutHit.TraceEnd);
				
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(FireMuzzle->GetComponentLocation());
				SpawnTransform.SetRotation(FQuat(BulletSpawnRotation));
				SpawnTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

				FActorSpawnParameters SpawnParameter;
				SpawnParameter.Owner = this;
				
				GetWorld()->SpawnActor<ALauncherBullet>(ALauncherBullet::StaticClass(), SpawnTransform, SpawnParameter);

				OwnerCharacter->LauncherFireEmitter_Server();
				OwnerCharacter->LauncherShootCameraShake_Client();
			}
			else
			{
				TimerManager.ClearTimer(FireEventHandle);
			}
		}
	}
}

void ALauncher::LauncherCoolTime()
{
	LauncherCoolTimeDone = true;
}
