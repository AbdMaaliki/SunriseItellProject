// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipCharacter.h"
#include "SunriseProjectProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "SpaceShipGameModeBase.h"
#include "SpaceShipPlayerController.h"
#include "SpaceShipPlayerState.h"
#include "SpaceShipGameStateBase.h"

// Sets default values
ASpaceShipCharacter::ASpaceShipCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Space Ship Mesh"));
	SpaceMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(SpaceMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

	bReplicates = true;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

}

// Replicated Properties
void ASpaceShipCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(ASpaceShipCharacter, CurrentHealth);
}

// Called when the game starts or when spawned
void ASpaceShipCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AssignMaterial();
	SetCurrentHealth(MaxHealth);
	MyController = Cast<ASpaceShipPlayerController>(GetController());
	if(MyController)
	{
		MyController->SetStartLocation(GetActorLocation());
		MyController->CallUpdateUIDelegate();
	}
}

// Called every frame
void ASpaceShipCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpaceShipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASpaceShipCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASpaceShipCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASpaceShipCharacter::ServerFire);
	
}


void ASpaceShipCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASpaceShipCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASpaceShipCharacter::MultiFire_Implementation()
{
	if(ProjectileClass)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		ASunriseProjectProjectile* Projectile = GetWorld()->SpawnActor<ASunriseProjectProjectile>(ProjectileClass, Location, Rotation);
		Projectile->SetOwner(this);
	}
}

void ASpaceShipCharacter::ServerFire_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		MultiFire();
	}
}


void ASpaceShipCharacter::Die_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		ASpaceShipGameModeBase* MyGameMode = Cast<ASpaceShipGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		ASpaceShipPlayerController* ShipController = Cast<ASpaceShipPlayerController>(GetController());
		
		if(ShipController && MyGameMode)
		{
			MyGameMode->CallRespawn(ShipController);
		}
		Destroy();
	}
}

void ASpaceShipCharacter::SetCurrentHealth(float NewHealth)
{
	if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
        OnHealthUpdate();
    }
}

void ASpaceShipCharacter::ReceiveDamage(AActor* DamageCauser, float DamageTaken)
{
	ASpaceShipCharacter* Shooter = Cast<ASpaceShipCharacter>(DamageCauser);
	if((Shooter) && (CheckTeam(Shooter, this)))
	{
    	float damageApplied = CurrentHealth - DamageTaken;

		if(CurrentHealth <= DamageTaken)
		{
			ASpaceShipPlayerState* ShooterState = Cast<ASpaceShipPlayerState>(Shooter->GetPlayerState());
			ASpaceShipGameStateBase* GameState = Cast<ASpaceShipGameStateBase>(GetWorld()->GetGameState());
			if(ShooterState && GameState)
			{
				ShooterState->IncrementPlayerKills(1);
				GameState->IncrementTeamKillCount(ShooterState);
			}
		}

		SetCurrentHealth(damageApplied);
	}
}

void ASpaceShipCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void ASpaceShipCharacter::OnHealthUpdate()
{
    //Client-specific functionality
    if (IsLocallyControlled())
    {
		if(MyController)
		{
			MyController->CallUpdateUIDelegate();
		}
        if (CurrentHealth <= 0)
        {
            Die();
        }
    }

    //Server-specific functionality
    if (GetLocalRole() == ROLE_Authority)
    {
    }
}


bool ASpaceShipCharacter::CheckTeam(ASpaceShipCharacter* Ship1, ASpaceShipCharacter* Ship2)
{
	ASpaceShipPlayerState* Ship1State = Cast<ASpaceShipPlayerState>(Ship1->GetPlayerState());
	ASpaceShipPlayerState* Ship2State = Cast<ASpaceShipPlayerState>(Ship2->GetPlayerState());
	if(Ship1State && Ship1State)
	{
		if(Ship1State->GetTeamName() != Ship2State->GetTeamName())
		{
			return true;
		}
	}
	return false;
}

