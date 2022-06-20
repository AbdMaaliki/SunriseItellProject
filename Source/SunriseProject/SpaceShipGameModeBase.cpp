// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipGameModeBase.h"
#include "SpaceShipCharacter.h"
#include "Kismet/GameplayStatics.h"


void ASpaceShipGameModeBase::CallRespawn(ASpaceShipPlayerController* PlayerController)
{   
    if(GetLocalRole() == ROLE_Authority)
    {
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASpaceShipGameModeBase::ServerRespawn, PlayerController);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 3, false);
    }
}


void ASpaceShipGameModeBase::ServerRespawn(ASpaceShipPlayerController* PlayerController)
{
    if(GetLocalRole() == ROLE_Authority)
    {
        if(ShipToSpawnClass && PlayerController)
        {
            FVector Location = PlayerController->GetSpawnLocation();
            FRotator Rotation = FRotator::ZeroRotator;
            ASpaceShipCharacter* RespawnedShip = GetWorld()->SpawnActor<ASpaceShipCharacter>(ShipToSpawnClass, Location, Rotation);
            if(RespawnedShip)
            {
                PlayerController->Possess(RespawnedShip);
            }
        }
    }
}


void ASpaceShipGameModeBase::Restart_Implementation()
{
    UGameplayStatics::OpenLevel(this, "TwinStickExampleMap");
}