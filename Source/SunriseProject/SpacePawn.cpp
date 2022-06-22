// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePawn.h"

#include "SunriseProjectProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASpacePawn::ASpacePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	bReplicates = true;
}


// Replicated Properties
void ASpacePawn::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASpacePawn, Speed);
	DOREPLIFETIME(ASpacePawn, TurnRate);
}

// Called when the game starts or when spawned
void ASpacePawn::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called every frame
void ASpacePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpacePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASpacePawn::Server_CallMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASpacePawn::Server_CallMoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ASpacePawn::Server_CallTurn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASpacePawn::Server_CallFire);

	
}

void ASpacePawn::Server_CallTurn_Implementation(float value)
{
	if(HasAuthority())
	{
		Turn(value);
	}
}

void ASpacePawn::Server_CallMoveRight_Implementation(float value)
{
	if(HasAuthority())
	{
		MoveRight(value);
	}
}

void ASpacePawn::Server_CallMoveForward_Implementation(float value)
{
	if(HasAuthority())
	{
		MoveForward(value);
	}
}

void ASpacePawn::MoveForward_Implementation(float value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	value = value * DeltaTime * Speed;
	FVector DeltaLocation(value, 0.f, 0.f);
	AddActorLocalOffset(DeltaLocation, true);
}


void ASpacePawn::MoveRight_Implementation(float value)
{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		value = value * DeltaTime * Speed;
		FVector DeltaLocation(0.f, value, 0.f);
		AddActorLocalOffset(DeltaLocation, true);
}

void ASpacePawn::Turn_Implementation(float value)
{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		value = value * DeltaTime * TurnRate;
		FRotator Rotation(0.f, value, 0.f);
		AddActorLocalRotation(Rotation, true);
}

void ASpacePawn::Server_CallFire_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		Multi_Fire();
	}
}
void ASpacePawn::Multi_Fire_Implementation()
{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

		if(IsValid(ProjectileClass))
		{
			auto Projectile = GetWorld()->SpawnActor<ASunriseProjectProjectile>(ProjectileClass, Location, Rotation);
			Projectile->SetOwner(this);
		}
}

