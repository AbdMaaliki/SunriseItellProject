// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SpaceShipGameStateBase.h"
#include "SpaceShipCharacter.h"


ASpaceShipPlayerState::ASpaceShipPlayerState()
{
    PlayerKills = 0;
}

void ASpaceShipPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ASpaceShipGameStateBase* MyGameState = Cast<ASpaceShipGameStateBase>(GetWorld()->GetGameState());
	if(MyGameState)
	{
		MyGameState->AddSpaceShip(this);
	}
}


// Replicated Properties
void ASpaceShipPlayerState::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate Kills.
    DOREPLIFETIME(ASpaceShipPlayerState, PlayerKills);
}

void ASpaceShipPlayerState::IncrementPlayerKills(int32 KillsCount)
{
    PlayerKills += KillsCount;
}

void ASpaceShipPlayerState::SetTeamName(FString NewTeamName)
{
	TeamName = NewTeamName;
}

void ASpaceShipPlayerState::ResetValues()
{
	PlayerKills = 0;
}