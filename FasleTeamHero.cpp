// Fill out your copyright notice in the Description page of Project Settings.


#include "FasleTeamHero.h"

AFasleTeamHero::AFasleTeamHero()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT(TEXT("Material'/Game/Characters/Materials/HeroTPP.HeroTPP'"));
	if (MAT.Succeeded())
	{
		GetMesh()->SetMaterial(0, MAT.Object);
	}
}