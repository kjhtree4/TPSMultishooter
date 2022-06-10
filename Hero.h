// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMultiShooter.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Class.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShake.h"
#include "Hero.generated.h"


UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Shooting UMETA(DisplayName = "Shooting"),
	Reloading UMETA(DisplayName = "Reloading"),
	Die UMETA(DisplayName = "Die"),
	Swapping UMETA(DisplayName = "Swapping")
};

DECLARE_MULTICAST_DELEGATE(FOnGetDamageDelegate);
DECLARE_MULTICAST_DELEGATE(FOnImpactPointChangeForUIDelegate);

UCLASS()
class TPSMULTISHOOTER_API AHero : public ACharacter
{
	GENERATED_BODY()

public:
		FOnGetDamageDelegate OnGetDamageDelegate;
		FOnImpactPointChangeForUIDelegate OnImpactPointChangeForUI;
public:
	UPROPERTY(EditAnywhere)
		USpringArmComponent *SpringArm;
	UPROPERTY(EditAnywhere)
		UCameraComponent *Camera;
	
	
public:
	// Sets default values for this character's properties
	AHero();
	

	bool IsEquipRiffle;
	bool IsEquipLauncher;
	bool CanShoot;
	bool CanReload;

	float Speed;
	int32 LowHPWidgetBaseLine;
	

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
		int32 CurrentHP;
	UFUNCTION()
		void OnRep_CurrentHP();

	UPROPERTY(Replicated)
		int32 MaxHP;

	UPROPERTY(EditAnywhere)
		EPlayerStatus PlayerStatus;

	UPROPERTY(EditAnywhere)
		class ARiffle *Riffle;
	UPROPERTY(EditAnywhere)
		class ALauncher *Launcher;

	UPROPERTY(EditAnywhere)
		class UHeroAnimInstance *HeroAnimInstance;

	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> LauncherCameraShake;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> RiffleCameraShake;

	UPROPERTY(EditAnywhere)
		class USoundBase *ImpactSound;
	UPROPERTY(EditAnywhere)
		class USoundBase *DieSound;

	bool WantToRun;
	bool Team;

	float MoveForwardAxisValue;

	UPROPERTY(Replicated)
		float AimPitch;

	float WalkSpeed;
	float RunSpeed;


	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UUserWidget> MainUIClass;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UUserWidget> LowHPUIClass;

	UPROPERTY(VisibleAnywhere)
		class UUserWidget *MainUI;//container of mainUI
	UPROPERTY(VisibleAnywhere)
		class UUserWidget *LowHPUI;//container of lowhpUI

	UPROPERTY(Replicated)
		FVector DamageCauserLocation;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectForRadialDamage;

	float RealDamage;
	bool IsLowHPWidgetAddToViewport;
	

	UPROPERTY(EditAnywhere)
		class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* DieMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* SwapMontage;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookUp(float Value);
	void LookRight(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Jumping();
	void Reload();
	void SwapWeapon_Pressed();
	

	UFUNCTION(Server, Reliable)
		void ChangeColByTeam_Server(bool TeamBool);
	UFUNCTION(Client, Reliable)
		void ChangeColByTeam_Client(bool TeamBool);

	UFUNCTION(Server, Reliable)
		void MaxWalkSpeed_Server();
	UFUNCTION(Client, Reliable)
		void MaxWalkSpeed_Client();

	UFUNCTION(Server, Reliable)
		void MaxRunSpeed_Server();
	UFUNCTION(Client, Reliable)
		void MaxRunSpeed_Client();
	

	

	UFUNCTION(Client, UnReliable)
		void PlayImpactSound_Client();
	UFUNCTION(Client, UnReliable)
		void PlayDieSound_Client();

	UFUNCTION(Server, Reliable)
		void PlayReloadMontage_Server();
	UFUNCTION(NetMulticast, Reliable)
		void PlayReloadMontage_Multi();

	UFUNCTION(Server, Reliable)
		void PlaySwapMontage_Server(); 
	UFUNCTION(NetMulticast, Reliable)
		void PlaySwapMontage_Multi();

	UFUNCTION(Server, Reliable)
		void SwapToLauncher_Server();
	UFUNCTION(Server, Reliable)
		void SwapToRiffle_Server();

	UFUNCTION(Client, Reliable)
		void ShowMainUI_Client();
	UFUNCTION(Client, Reliable)
		void RemoveMainUI_Client();

	UFUNCTION(Client, Reliable)
		void GetDamageDelegateCall_Client();
	UFUNCTION(Client, Reliable)
		void HitIndicatorCall_Client();

	UFUNCTION(Client, UnReliable)
		void PlayItemSound_Client(class USoundBase *Sound);
	
	UFUNCTION(Client, Reliable)
		void Die_Client();

	UFUNCTION(Server, Reliable)
		void PlayDieMontage_Server();
	UFUNCTION(NetMulticast, Reliable)
		void PlayDieMontage_Multi();

	UFUNCTION(Client, Reliable)
		void ShowLowHPWidget_Client();
	UFUNCTION(Client, Reliable)
		void RemoveLowHPWidget_Client();

	UFUNCTION(Server, Reliable)
		void BulletHitProcess_Server(FVector ImpactPoint, AActor *HittedActor, int32 WeaponNum);
	UFUNCTION(Server, UnReliable)
		void RiffleFireEmitter_Server();
	UFUNCTION(Server, UnReliable)
		void LauncherFireEmitter_Server();

	UFUNCTION(Client, UnReliable)
		void LauncherShootCameraShake_Client();
	UFUNCTION(Client, UnReliable)
		void LauncherBoomCameraShake_Client();
	UFUNCTION(Client, UnReliable)
		void RiffleShootCameraShake_Client();


	UFUNCTION(Server, Reliable)
		void AddHP_Server(int32 AddHP);
	UFUNCTION(Client, Reliable)
		void AddRiffleBullet_Client(int32 AddBullet);
	UFUNCTION(Client, Reliable)
		void AddLauncherBullet_Client(int32 AddBullet);


private:
	void Shooting_Pressed();
	void Shooting_Released();

	void Run_Pressed();
	void Run_Released();

	UFUNCTION(NetMulticast, Reliable)
		void SetUseControlRotationYawInAnimInstance_Multi();


	UFUNCTION()
		void SpawnWeapons();

	UFUNCTION(Server, Reliable)
		void GetRadialDamage_Server(float ActualDamage, FVector Origin, bool TeamBool);

	UFUNCTION(Server, Reliable)
		void CalculateRadialDamage_Server(float BaseDamage, FVector Origin, float DamagaRadius);
	

	UFUNCTION(Server, Reliable)
		void ApplyDamage_Server(float Damage, FVector DamageCausersLocation, bool TeamBool);

};
