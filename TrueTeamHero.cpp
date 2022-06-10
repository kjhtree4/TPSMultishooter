// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueTeamHero.h"

ATrueTeamHero::ATrueTeamHero()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT(TEXT("Material'/Game/Characters/Materials/HeroTPP_2.HeroTPP_2'"));
	if (MAT.Succeeded())
	{
		GetMesh()->SetMaterial(0, MAT.Object);
	}
}