// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "HeroPlayController.h"
#include "Net/UnrealNetwork.h"
#include "Item.h"



AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particlesystem"));
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (SM.Succeeded())
	{
		Floor->SetStaticMesh(SM.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT(TEXT("MaterialInstanceConstant'/Game/CloseCombat/Maps/Materials/M_Color_Inst1.M_Color_Inst1'"));
	if (MAT.Succeeded())
	{
		Floor->SetMaterial(0,MAT.Object);
	}


	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemBeginOverlap);

	RootComponent = SphereCol;
	ParticleSystem->SetupAttachment(SphereCol);
	Floor->SetupAttachment(SphereCol);

	SphereCol->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	ParticleSystem->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
	ParticleSystem->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	Floor->SetRelativeLocation(FVector(0.f, 0.f, -27.f));
	Floor->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.1f));

	SphereCol->SetCollisionProfileName(FName(TEXT("ItemCol")));

	IsActive = true;

	bReplicates = true;
	
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItem, IsActive);
	
}

void AItem::OnItemBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	if (HasAuthority())
	{
		if (IsActive)
		{
			if (Cast<AHero>(OtherActor))
			{
				AHero *OverlappedHero = Cast<AHero>(OtherActor);
				if (CanBeEaten(OverlappedHero))
				{
					if (Cast<AHeroPlayController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
					{
						Controller = Cast<AHeroPlayController>(OverlappedHero->GetController());
						Controller->SayToClient(ItemGetString);

						FTimerManager &TimerManager = GetWorld()->GetTimerManager();
						TimerManager.SetTimer(NotificationClearHandle, this, &AItem::NotificationClear, 1.5f, false);

						OverlappedHero->PlayItemSound_Client(Sound);
						ItemFunction(OverlappedHero);
						IsActive = false;

						TimerManager.SetTimer(RespawnTimerHandle, this, &AItem::Respawn, RespawnTime, false);
					}
				}

			}
		}
	}
	
}

void AItem::ItemFunction(class AHero *OverlappedActor)
{
}

bool AItem::CanBeEaten(class AHero *Hero)
{

	return true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Respawn()
{
	IsActive = true;
}

void AItem::NotificationClear()
{
	Controller->SayToClient(FString(TEXT("")));
}

void AItem::OnRep_IsActive()
{
	if (IsActive)
	{
		ParticleSystem->SetVisibility(true);
	}
	else 
	{
		ParticleSystem->SetVisibility(false);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

