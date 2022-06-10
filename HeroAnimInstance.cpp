// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Riffle.h"
#include "Launcher.h"
#include "HeroAnimInstance.h"

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Hero = Cast<AHero>(Pawn);

		Speed = Pawn->GetVelocity().Size();
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		AimPitch = Hero->AimPitch;

		IsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
	
}

void UHeroAnimInstance::AnimNotify_ReloadEnd()
{
	Hero->PlayerStatus = EPlayerStatus::Idle;
	if (Hero->IsEquipRiffle)
	{
		if (IsValid(Hero->Riffle))
			Hero->Riffle->OnRiffleFireDelegate.Broadcast();
	}
	else if(Hero->IsEquipLauncher)
	{
		if (IsValid(Hero->Launcher))
			Hero->Launcher->OnLauncherFireDelegate.Broadcast();
	}
}

void UHeroAnimInstance::AnimNotify_SwapEnd()
{
	Hero->PlayerStatus = EPlayerStatus::Idle;
}



