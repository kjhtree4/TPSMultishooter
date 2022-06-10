// Fill out your copyright notice in the Description page of Project Settings.


#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Hero.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetMathLibrary.h"
#include "HitIndicatorWidget.h"

void UHitIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Material = IndicatorBorder->GetDynamicMaterial();
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		HeroCharacter = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		HeroCharacter->OnImpactPointChangeForUI.AddUObject(this, &UHitIndicatorWidget::Animation);
		IndicatorBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHitIndicatorWidget::NativeTick(const FGeometry & MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	FVector Normalize = UKismetMathLibrary::Normal(HeroCharacter->GetActorLocation() - HeroCharacter->DamageCauserLocation, 0.001f);

	float X = UKismetMathLibrary::Dot_VectorVector(HeroCharacter->GetActorForwardVector(), Normalize);
	
	float Y = (HeroCharacter->GetActorForwardVector().X * Normalize.Y) - (HeroCharacter->GetActorForwardVector().Y * Normalize.X);
	
	float Value = UKismetMathLibrary::DegAtan2(Y,X);

	float DegreeValue = UKismetMathLibrary::MapRangeClamped(Value, 180.f, -180.f, 0.f, 1.f);
	
	Material->SetScalarParameterValue(FName(TEXT("Angle")), DegreeValue);
}

void UHitIndicatorWidget::Animation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Animation!"));

	if(StartAnim)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Animation Valid"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("AnimationNotValid"));

	IndicatorBorder->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(StartAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
}