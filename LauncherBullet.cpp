// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Hero.h"
#include "Launcher.h"
#include "LauncherBullet.h"

// Sets default values
ALauncherBullet::ALauncherBullet()
{

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticlesystemComponent"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/RocketLauncher/Muzzle/P_Launcher_proj.P_Launcher_proj'"));
	if (Particle.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(Particle.Object);
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALauncherBullet::OnBulletBeginOverlap);


	Sphere->SetCollisionProfileName(FName(TEXT("BulletCollision")));

	RootComponent = Sphere;
	ParticleSystemComponent->SetupAttachment(Sphere);
	Arrow->SetupAttachment(Sphere);

	bReplicates = true;

	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->MaxSpeed = 1500.f;

}

// Called when the game starts or when spawned
void ALauncherBullet::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		Launcher = Cast<ALauncher>(GetOwner());
	}
}

void ALauncherBullet::OnBulletBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Launcher->OwnerCharacter->BulletHitProcess_Server(Sphere->GetComponentLocation(), OtherActor, 2);

	this->Destroy();
}


