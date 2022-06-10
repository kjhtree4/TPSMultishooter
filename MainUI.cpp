// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Riffle.h"
#include "Launcher.h"
#include "HeroPlayController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainUI.h"



void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		BasicAimSize = FVector2D(1.f, 1.f);

		HeroCharacter = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		Riffle = HeroCharacter->Riffle;
		Launcher = HeroCharacter->Launcher;

		if (IsValid(Riffle) && IsValid(Launcher)) {
			GettingBulletInfo();
			GettingHP();
			SettingBulletText();
			SettingHP();

			Riffle->OnRiffleFireDelegate.AddUObject(this, &UMainUI::GettingBulletInfo);
			Launcher->OnLauncherFireDelegate.AddUObject(this, &UMainUI::GettingBulletInfo);
			HeroCharacter->OnGetDamageDelegate.AddUObject(this, &UMainUI::GettingHP);
		}
	}
}

void UMainUI::NativeTick(const FGeometry & MyGeometry, float DeltaTime)
{           
	Super::NativeTick(MyGeometry, DeltaTime);

	if (IsValid(Riffle))
	{
		TickTime = DeltaTime;
		AimImage->SetRenderScale(FVector2D(UKismetMathLibrary::ClampAngle(Riffle->AimSpread, 0.f, 1.f))+BasicAimSize);
	}
	
}

void UMainUI::SettingBulletText()
{
	Text_BulletInAmmo_Launcher->SetText(FText::AsNumber(BulletInAmmo_Launcher));
	Text_CurrentBullet_Launcher->SetText(FText::AsNumber(CurrenBullet_Launcher));
	Text_BulletInAmmo_Riffle->SetText(FText::AsNumber(BulletInAmmo_Riffle));
	Text_CurrentBullet_Riffle->SetText(FText::AsNumber(CurrenBullet_Riffle));
}

void UMainUI::GettingBulletInfo()
{
	BulletInAmmo_Riffle = Riffle->BulletInAmmo;
	CurrenBullet_Riffle = Riffle->CurrentBullet;
	MaxBullet_Riffle = Riffle->MaxBullet;

	BulletInAmmo_Launcher = Launcher->BulletInAmmo;
	CurrenBullet_Launcher = Launcher->CurrentBullet;
	MaxBullet_Launcher = Launcher->MaxBullet;

	SettingBulletText();
}

void UMainUI::GettingHP()
{
	CurrentHP = HeroCharacter->CurrentHP;
	MaxHp = HeroCharacter->MaxHP;

	SettingHP();
}

void UMainUI::SettingHP()
{
	Text_CurrentHP->SetText(FText::AsNumber(CurrentHP));
	HPBar->SetPercent(float(CurrentHP) / float(MaxHp));
}