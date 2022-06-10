// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "MainUI.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 //	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	FireMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("FireMuzzle"));

	RootComponent = SkeletalMesh;
	FireMuzzle->SetupAttachment(SkeletalMesh);

	SkeletalMesh->SetCollisionProfileName(FName("NoCollision"));

	ObjectTypeForLinetrace.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypeForLinetrace.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ObjectTypeForLinetrace.Add(EObjectTypeQuery::ObjectTypeQuery9);

	FireMuzzle->SetRelativeLocation(FVector(0.f, 80.f, 0.f));
	FireMuzzle->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USoundBase> NoAmmoS(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/WeaponAmmoEmpty_Cue.WeaponAmmoEmpty_Cue'"));
	if (NoAmmoS.Succeeded())
	{
		NoAmmoSound = NoAmmoS.Object;
	}

	bReplicates = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentBullet = MaxBullet;
	BulletInAmmo = MaxBullet;

	if(Cast<AHero>(GetOwner()))
		OwnerCharacter = Cast<AHero>(GetOwner());
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimSpread = UKismetMathLibrary::FInterpTo(AimSpread, 0.f, DeltaTime, 6.f);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, MaxBullet);
	DOREPLIFETIME(AWeapon, ImpactPoint);
	
}

void AWeapon::AttachToOwnerHand()
{
	if (Cast<AHero>(GetOwner()))
	{
		AHero *Hero = Cast<AHero>(GetOwner());
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);;
		this->AttachToComponent(Hero->GetMesh(),AttachmentTransformRules,FName(TEXT("WeaponPoint")));
	}
}

void AWeapon::AttachToOwnerBack()
{
	if (Cast<AHero>(GetOwner()))
	{
		AHero *Hero = Cast<AHero>(GetOwner());
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);;
		this->AttachToComponent(Hero->GetMesh(), AttachmentTransformRules, FName(TEXT("WeaponPoint2")));
	}
}


void AWeapon::Fire()
{
	if (CheckBulletWhenShooting())
	{
		OwnerCharacter->PlayerStatus = EPlayerStatus::Shooting;
		FireEvent();

		FTimerManager &TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(FireEventHandle, this, &AWeapon::FireEvent, FireTerm, true);
	}
}

void AWeapon::FireEvent()
{
}

void AWeapon::SpawnHeroHitEmitter_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HeroHitEmitter, ImpactPoint, FRotator(0.f));
}

void AWeapon::SpawnObjectHitSound_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, ImpactPoint);
}

void AWeapon::SpawnObjectHitEmitter_Implementation()
{
	FString forDebug = ImpactPoint.ToString();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ObjectHitEmitter, ImpactPoint, FRotator(0.f));
}

void AWeapon::SpawnFireSound_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, FireMuzzle->GetComponentLocation());
}

void AWeapon::SpawnFireEmitter_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEmitter, FireMuzzle->GetComponentLocation(), UKismetMathLibrary::MakeRotFromX(FireMuzzle->GetForwardVector()));
}

void AWeapon::AimSpreadFunc_Implementation()
{

	if (AimSpread < 10.f)
	{
		AimSpread = AimSpread + 1.5;
	}
}

void AWeapon::Reload()
{
	if (CurrentBullet < MaxBullet)
	{
		if (BulletInAmmo > 0)
		{
			if ((MaxBullet - CurrentBullet) < BulletInAmmo)
			{
				BulletInAmmo = BulletInAmmo - (MaxBullet - CurrentBullet);
				CurrentBullet = MaxBullet;
			}
			else
			{
				CurrentBullet = CurrentBullet + BulletInAmmo;
				BulletInAmmo = 0;
			}

			OwnerCharacter->PlayerStatus = EPlayerStatus::Reloading;
			OwnerCharacter->PlayReloadMontage_Server();
		}
	}
}

void AWeapon::NoAmmo()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, FireMuzzle->GetComponentLocation());

	FTimerManager &TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(FireEventHandle);
}

bool AWeapon::CheckBulletWhenShooting()
{
	bool CanShoot = false;

	if (CurrentBullet > 0)
	{
		CanShoot = true;
	}
	else
	{
		CanShoot = false;
		if (BulletInAmmo > 0)
		{
			OwnerCharacter->Reload();
		}
		else
		{
			NoAmmo();
		}
	}
	return CanShoot;
}


