
// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Riffle.h"
#include "Launcher.h"
#include "Net/UnrealNetwork.h"
#include "HeroAnimInstance.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "HeroPlayController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Hero.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0.f, 45.f, 88.f));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	GetCharacterMovement()->MaxWalkSpeed = 360.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Characters/HeroTPP/HeroTPP.HeroTPP'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> HeroAnim(TEXT("AnimBlueprint'/Game/Animations/HeroAnimInstance.HeroAnimInstance_C'"));
	if (HeroAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HeroAnim.Class);
	}
	
	static ConstructorHelpers::FClassFinder<UCameraShake> LauncherShake(TEXT("Blueprint'/Game/MyWorks/LauncherCameraShake.LauncherCameraShake_C'"));
	if (LauncherShake.Succeeded())
	{
		LauncherCameraShake = LauncherShake.Class;
	}

	static ConstructorHelpers::FClassFinder<UCameraShake> RiffleShake(TEXT("Blueprint'/Game/MyWorks/RiffleCameraShake.RiffleCameraShake_C'"));
	if (RiffleShake.Succeeded())
	{
		RiffleCameraShake = RiffleShake.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Impact(TEXT("SoundCue'/Game/Sounds/FoleyFlesh/Mono/FoleyFlesh_Impact_Cue.FoleyFlesh_Impact_Cue'"));
	if (Impact.Succeeded())
	{
		ImpactSound = Impact.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Die(TEXT("SoundCue'/Game/Sounds/Stingers/Stereo/Stinger_PlayerDeath_Stereo_Cue.Stinger_PlayerDeath_Stereo_Cue'"));
	if (Die.Succeeded())
	{
		DieSound = Die.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>  Reload(TEXT("AnimMontage'/Game/Animations/TTP_Animations/ReloadMontage.ReloadMontage'"));
	if (Reload.Succeeded())
	{
		ReloadMontage = Reload.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>  DieMon(TEXT("AnimMontage'/Game/Animations/TTP_Animations/DieMontage.DieMontage'"));
	if (Reload.Succeeded())
	{
		DieMontage = DieMon.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwapMon(TEXT("AnimMontage'/Game/Animations/TTP_Animations/SwapMontage.SwapMontage'"));
	if (SwapMon.Succeeded())
	{
		SwapMontage = SwapMon.Object;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> Main(TEXT("WidgetBlueprint'/Game/Widgets/MainUI.MainUI_C'"));
	if (Main.Succeeded())
	{
		MainUIClass=Main.Class;
	}
	

	static ConstructorHelpers::FClassFinder<UUserWidget> LowHP(TEXT("WidgetBlueprint'/Game/Widgets/LowHpWidget.LowHpWidget_C'"));
	if (LowHP.Succeeded())
	{
		LowHPUIClass = LowHP.Class;
	}
	
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	
	
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("TrueHeroCol")));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	IsEquipRiffle = true;
	IsEquipLauncher = false;
	CanShoot = false;
	CanReload = false;
	WantToRun = false;
	
	LowHPWidgetBaseLine = 20;
	WalkSpeed = 360.f;
	RunSpeed = 730.f;
	MaxHP = 100;
	PlayerStatus = EPlayerStatus::Idle;

	
	bUseControllerRotationYaw = true;
	SpringArm->bUsePawnControlRotation = true;

	ObjectForRadialDamage.Add(EObjectTypeQuery::ObjectTypeQuery9);
	ObjectForRadialDamage.Add(EObjectTypeQuery::ObjectTypeQuery12);
}

void AHero::OnRep_CurrentHP()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("HPUpdate"));
	GetDamageDelegateCall_Client();
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
	HeroAnimInstance = Cast<UHeroAnimInstance>(GetMesh()->GetAnimInstance());

	if (HasAuthority())//server
	{
		SpawnWeapons();
	}
	if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0))//MyPC
	{
		SpawnWeapons();

		ShowMainUI_Client();
	}
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		AimPitch = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Pitch;
	}
}

void AHero::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHero, CurrentHP);
	DOREPLIFETIME(AHero, MaxHP);
	DOREPLIFETIME(AHero, AimPitch);
	DOREPLIFETIME(AHero, DamageCauserLocation);
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &AHero::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AHero::LookRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Moveright"), this, &AHero::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AHero::Jumping);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AHero::Shooting_Pressed);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AHero::Shooting_Released);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AHero::Reload);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AHero::Run_Pressed);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AHero::Run_Released);
	PlayerInputComponent->BindAction(TEXT("SwapWeapon"), EInputEvent::IE_Pressed, this, &AHero::SwapWeapon_Pressed);

}


void AHero::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AHero::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void AHero::MoveForward(float Value)
{
	if (PlayerStatus != EPlayerStatus::Die)
	{
		MoveForwardAxisValue = Value;
		
		AddMovementInput(GetActorForwardVector(), Value);
		
		if (PlayerStatus == EPlayerStatus::Shooting) {
			MaxWalkSpeed_Server();
		}
		else {
			if (MoveForwardAxisValue < 0.f) {
				MaxWalkSpeed_Server();
			}
			else {
				if (WantToRun) {
					MaxRunSpeed_Server();
				}
				else {
					MaxWalkSpeed_Server();
				}
			}
		}
	}
}

void AHero::MoveRight(float Value)
{
	if (PlayerStatus != EPlayerStatus::Die) {
		this->AddMovementInput(GetActorRightVector(), Value);
	}

}

void AHero::Jumping()
{
	if (PlayerStatus != EPlayerStatus::Die)
		Jump();
}



void AHero::Reload()
{
	if (PlayerStatus != EPlayerStatus::Die && PlayerStatus != EPlayerStatus::Swapping)
	{
		if (IsEquipRiffle)
			Riffle->Reload();
		else if (IsEquipLauncher)
			Launcher->Reload();
	}
}

void AHero::SwapWeapon_Pressed()
{
	if(PlayerStatus != EPlayerStatus::Die && PlayerStatus != EPlayerStatus::Swapping && PlayerStatus != EPlayerStatus::Reloading)
	{
		PlayerStatus = EPlayerStatus::Swapping;
		if (IsEquipRiffle)
		{
			IsEquipRiffle = false;
			IsEquipLauncher = true;
			PlaySwapMontage_Server();
			SwapToLauncher_Server();
			Launcher->AttachToOwnerHand();
			Riffle->AttachToOwnerBack();
		}
		else if (IsEquipLauncher)
		{
			IsEquipRiffle = true;
			IsEquipLauncher = false;
			PlaySwapMontage_Server();
			SwapToRiffle_Server();
			Riffle->AttachToOwnerHand();
			Launcher->AttachToOwnerBack();
		}

	}
}

void AHero::ChangeColByTeam_Client_Implementation(bool TeamBool)
{
	if (TeamBool)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("TrueHeroCol")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SetTrueHeroCol"));
	}
	else
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("FalseHeroCol")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SetFalseHeroCol"));
	}
}

void AHero::ChangeColByTeam_Server_Implementation(bool TeamBool)
{
	if (TeamBool)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("TrueHeroCol")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SetTrueHeroCol"));
	}
	else
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("FalseHeroCol")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SetFalseHeroCol"));
	}

	ChangeColByTeam_Client(TeamBool);
}

void AHero::MaxWalkSpeed_Client_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AHero::MaxWalkSpeed_Server_Implementation()
{
	MaxWalkSpeed_Client();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AHero::MaxRunSpeed_Server_Implementation()
{
	MaxRunSpeed_Client();
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AHero::MaxRunSpeed_Client_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AHero::PlayImpactSound_Client_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound);
}

void AHero::PlayDieSound_Client_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);
}



void AHero::PlaySwapMontage_Multi_Implementation()
{
	HeroAnimInstance->Montage_Play(SwapMontage);
}

void AHero::SwapToLauncher_Server_Implementation()
{
	Launcher->AttachToOwnerHand();
	Riffle->AttachToOwnerBack();
}

void AHero::SwapToRiffle_Server_Implementation()
{
	Riffle->AttachToOwnerHand();
	Launcher->AttachToOwnerBack();
}

void AHero::ShowMainUI_Client_Implementation()
{
		if (MainUI == nullptr)
		{
			AHeroPlayController *HeroPlayerController;
			HeroPlayerController = Cast<AHeroPlayController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (HeroPlayerController)
			{
				MainUI = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), MainUIClass);
				if (MainUI != nullptr) {
					MainUI->AddToViewport();
				}
			}
		}
}

void AHero::RemoveMainUI_Client_Implementation()
{
	if (MainUI != nullptr)
	{
		MainUI->RemoveFromParent();
	}
}

void AHero::GetDamageDelegateCall_Client_Implementation()
{
	OnGetDamageDelegate.Broadcast();
}

void AHero::HitIndicatorCall_Client_Implementation()
{
	OnImpactPointChangeForUI.Broadcast();
}

void AHero::PlayItemSound_Client_Implementation(USoundBase *Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}


void AHero::Die_Client_Implementation()
{
	if (PlayerStatus != EPlayerStatus::Die)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		PlayerStatus = EPlayerStatus::Die;
		bUseControllerRotationYaw = false;
		PlayDieMontage_Server();
	}
}

void AHero::PlayDieMontage_Server_Implementation()
{
	PlayDieMontage_Multi();

	AHeroPlayController *PlayerController = Cast<AHeroPlayController>(GetController());
	PlayerController->RespawnTimerExecuter_Server();

}

void AHero::PlayDieMontage_Multi_Implementation()
{
	HeroAnimInstance->Montage_Play(DieMontage);
}

void AHero::ShowLowHPWidget_Client_Implementation()
{
	if (!IsLowHPWidgetAddToViewport)
	{
		if (IsValid(LowHPUI))
		{
			LowHPUI->AddToViewport();
		}
		else
		{
			LowHPUI = CreateWidget(GetWorld()->GetFirstPlayerController(), LowHPUIClass);
			if (LowHPUI != nullptr)
				LowHPUI->AddToViewport();
		}
		IsLowHPWidgetAddToViewport = true;
	}
}

void AHero::RemoveLowHPWidget_Client_Implementation()
{
	if (IsLowHPWidgetAddToViewport)
	{
		if (IsValid(LowHPUI))
		{
			LowHPUI->RemoveFromParent();
			IsLowHPWidgetAddToViewport = false;
		}
	}
}

void AHero::BulletHitProcess_Server_Implementation(FVector ImpactPoint, AActor *HittedActor, int32 WeaponNum)
{

	if (WeaponNum == 1)
	{
		Riffle->ImpactPoint = ImpactPoint;
		RiffleFireEmitter_Server();

		if (Cast<AHero>(HittedActor))
		{
			AHero *Hero = Cast<AHero>(HittedActor);
			Riffle->SpawnHeroHitEmitter();
			Hero->ApplyDamage_Server(Riffle->Damage, this->GetActorLocation(), Team);
		}
		else
		{
			Riffle->SpawnObjectHitEmitter();
			Riffle->SpawnObjectHitSound();
		}
	}
	else if (WeaponNum == 2)
	{
		Launcher->ImpactPoint = ImpactPoint;
		LauncherFireEmitter_Server();

		if (Cast<AHero>(HittedActor))
		{
			Launcher->SpawnHeroHitEmitter();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Herohitted"));
		}
		else
		{
			Launcher->SpawnObjectHitEmitter();
			Launcher->SpawnObjectHitSound();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("ObjectHitted"));
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), ImpactPoint.X));
		this->CalculateRadialDamage_Server(Launcher->Damage, ImpactPoint, Launcher->DamageRadius);
		
	}
}

void AHero::LauncherFireEmitter_Server_Implementation()
{
	Launcher->SpawnFireEmitter();
	Launcher->SpawnFireSound();
}

void AHero::LauncherBoomCameraShake_Client_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), LauncherCameraShake, GetActorLocation(), 0.f, 500.f);
}

void AHero::AddHP_Server_Implementation(int32 AddHP)
{
	if ((AddHP + CurrentHP) > MaxHP)
	{
		CurrentHP = MaxHP;
	}
	else
	{
		CurrentHP = AddHP + CurrentHP;
		RemoveLowHPWidget_Client();
	}
}

void AHero::AddRiffleBullet_Client_Implementation(int32 AddBullet)
{
	if (IsValid(Riffle))
	{
		int32 CurrenAmmo = Riffle->BulletInAmmo;
		Riffle->BulletInAmmo = CurrenAmmo + AddBullet;
		Riffle->OnRiffleFireDelegate.Broadcast();
	}
}

void AHero::AddLauncherBullet_Client_Implementation(int32 AddBullet)
{
	if (IsValid(Launcher))
	{
		int32 CurrenAmmo = Launcher->BulletInAmmo;
		Launcher->BulletInAmmo = CurrenAmmo + AddBullet;
		Launcher->OnLauncherFireDelegate.Broadcast();
	}
}

void AHero::Shooting_Pressed()
{

	if (PlayerStatus != EPlayerStatus::Reloading && PlayerStatus != EPlayerStatus::Die && PlayerStatus != EPlayerStatus::Swapping)
	{
		if (IsEquipRiffle)
			Riffle->Fire();
		else if (IsEquipLauncher)
			Launcher->Fire();
	}
}

void AHero::RiffleFireEmitter_Server_Implementation()
{
	Riffle->SpawnFireEmitter();
	Riffle->SpawnFireSound();
}

void AHero::LauncherShootCameraShake_Client_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), LauncherCameraShake, GetActorLocation(), 0.f, 500.f);
}

void AHero::Shooting_Released()
{
	if (PlayerStatus != EPlayerStatus::Die && PlayerStatus != EPlayerStatus::Reloading)
	{
		PlayerStatus = EPlayerStatus::Idle;
		if (IsEquipRiffle) {
			FTimerManager &TimerManager = GetWorld()->GetTimerManager();
			TimerManager.ClearTimer(Riffle->FireEventHandle);
		}
		else if(IsEquipLauncher)
		{
			FTimerManager &TimerManager = GetWorld()->GetTimerManager();
			TimerManager.ClearTimer(Launcher->FireEventHandle);
		}
	}
}

void AHero::Run_Pressed()
{
	WantToRun = true;
}

void AHero::Run_Released()
{
	WantToRun = false;
}

void AHero::SetUseControlRotationYawInAnimInstance_Multi_Implementation()
{
	HeroAnimInstance->UseControllerRotationYaw = false;
}

void AHero::SpawnWeapons()
{

		FName WeaponSocket(TEXT("WeaponPoint"));
		FTransform SpawnTransform = GetMesh()->GetSocketTransform(WeaponSocket);

		FName WeaponSocket2(TEXT("WeaponPoint2"));
		FTransform SpawnTransform2 = GetMesh()->GetSocketTransform(WeaponSocket2);

		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;

		Riffle = GetWorld()->SpawnActor<ARiffle>(ARiffle::StaticClass(), SpawnTransform, SpawnParameter);
		if (IsValid(Riffle)) {
			Riffle->AttachToOwnerHand();
		}

		Launcher = GetWorld()->SpawnActor<ALauncher>(ALauncher::StaticClass(), SpawnTransform2, SpawnParameter);
		if (IsValid(Launcher)) {
			Launcher->AttachToOwnerBack();
		}
		
	
}




void AHero::GetRadialDamage_Server_Implementation(float ActualDamage, FVector Origin, bool TeamBool)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GetRadialDamage!"));

	if (TeamBool != Team) {
		DamageCauserLocation = Origin;

		HitIndicatorCall_Client();

		if ((CurrentHP - int32(ActualDamage)) > 0)
		{
			CurrentHP = CurrentHP - int32(ActualDamage);
			if (CurrentHP <= LowHPWidgetBaseLine) {
				ShowLowHPWidget_Client();
				LauncherBoomCameraShake_Client();
			}
			else
				LauncherBoomCameraShake_Client();

		}
		else
		{
			CurrentHP = 0;
			Die_Client();
			LauncherBoomCameraShake_Client();
		}
	}

}

void AHero::CalculateRadialDamage_Server_Implementation(float BaseDamage, FVector Origin, float DamagaRadius)
{

	TArray<AActor *> ActorToIgnore;
	TArray<FHitResult> OutHits;
	bool bResult = UKismetSystemLibrary::SphereTraceMultiForObjects(this->GetWorld(), Origin, Origin, DamagaRadius,
		ObjectForRadialDamage, true , ActorToIgnore, EDrawDebugTrace::Persistent, OutHits, true);

	if (bResult)
	{
		for (auto OutHit : OutHits)
		{
			AHero *HitActor;
			if (Cast<AHero>(OutHit.Actor))
			{
				HitActor = Cast<AHero>(OutHit.Actor);
				RealDamage = BaseDamage - ((Origin - OutHit.ImpactPoint).Size()*(BaseDamage / DamagaRadius));
				HitActor -> GetRadialDamage_Server(RealDamage, Origin,Team);
			}
		}
	}
	
}


void AHero::ApplyDamage_Server_Implementation(float Damage, FVector DamageCausersLocation, bool TeamBool)
{
	if (TeamBool != Team) {
		DamageCauserLocation = DamageCausersLocation;

		HitIndicatorCall_Client();

		if ((CurrentHP - Damage) > 0)
		{
			CurrentHP = CurrentHP - Damage;
			PlayImpactSound_Client();
			if (CurrentHP <= LowHPWidgetBaseLine)
				ShowLowHPWidget_Client();
		}
		else
		{
			CurrentHP = 0;
			PlayDieSound_Client();
			Die_Client();
			bUseControllerRotationYaw = false;
			SetUseControlRotationYawInAnimInstance_Multi();
		}
	}
}


void AHero::PlayReloadMontage_Server_Implementation()
{
	PlayReloadMontage_Multi();
}

void AHero::PlayReloadMontage_Multi_Implementation()
{
	HeroAnimInstance->Montage_Play(ReloadMontage);
}

void AHero::PlaySwapMontage_Server_Implementation()
{
	PlaySwapMontage_Multi();
}

void AHero::RiffleShootCameraShake_Client_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), RiffleCameraShake, Camera->GetComponentLocation() , 0.f, 50.f);
}
