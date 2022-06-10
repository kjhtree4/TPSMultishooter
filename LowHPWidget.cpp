// Fill out your copyright notice in the Description page of Project Settings.

#include "Sound/SoundBase.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "LowHPWidget.h"


ULowHPWidget::ULowHPWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	LowHpSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LowHPSoundComponent"));

	static ConstructorHelpers::FObjectFinder<USoundBase>Sound(TEXT("SoundCue'/Game/Sounds/InterfaceAudio/Mono/UI_LowHealth_Loop_Cue.UI_LowHealth_Loop_Cue'"));
	if (Sound.Succeeded())
	{
		LowHpSound = Sound.Object;
	}
}

void ULowHPWidget::NativeConstruct()
{
	PlayAnimation(FadeIn,0.f,0,EUMGSequencePlayMode::PingPong,1.0f,false);
	LowHpSoundComponent = UGameplayStatics::SpawnSound2D(GetWorld(),LowHpSound);
}

void ULowHPWidget::AnimationStop()
{
	PlayAnimation(FadeOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
	if (IsValid(LowHpSoundComponent))
	{
		LowHpSoundComponent->Stop();
	}
}

void ULowHPWidget::NativeDestruct()
{
	if (IsValid(LowHpSoundComponent))
	{
		LowHpSoundComponent->Stop();
	}
}