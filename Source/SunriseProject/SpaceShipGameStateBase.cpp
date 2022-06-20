// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShipCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SpaceShipPlayerController.h"
#include "SpaceShipGameModeBase.h"
#include "SpaceShipPlayerState.h"


ASpaceShipGameStateBase::ASpaceShipGameStateBase()
{
    TeamsStructA.TeamName = TEXT("Team A");
    TeamsStructA.TeamKills = 0;

    TeamsStructB.TeamName = TEXT("Team B");
    TeamsStructB.TeamKills = 0;
}

// Replicated Properties
void ASpaceShipGameStateBase::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASpaceShipGameStateBase, TeamsStructA);
    DOREPLIFETIME(ASpaceShipGameStateBase, TeamsStructB);
    DOREPLIFETIME(ASpaceShipGameStateBase, bGameRunning);
    DOREPLIFETIME(ASpaceShipGameStateBase, TimeRemaining);
}


void ASpaceShipGameStateBase::BeginPlay()
{
    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if(ASpaceShipGameModeBase* MyGameMode = Cast<ASpaceShipGameModeBase>(GameMode))
	{
		TimeRemaining = MyGameMode->GameLength;
	}

    FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpaceShipGameStateBase::CountDown, 1.f, bGameRunning, 0.0);
}


void ASpaceShipGameStateBase::AddSpaceShip(ASpaceShipPlayerState* SpaceShipState)
{
    FString NewName;
    if(SpaceShipState)
    
    if(SpaceShipsInGame.Num())
    {
        int32 LastIndex = SpaceShipsInGame.Num() - 1;
        if(SpaceShipsInGame[LastIndex]->GetTeamName() == TeamsStructA.TeamName)
        {
            NewName = TeamsStructB.TeamName;
        }
        else
        {
            NewName = TeamsStructA.TeamName;
        }

        LastIndex++;
        SpaceShipsInGame.Add(SpaceShipState);
        SpaceShipsInGame[LastIndex]->SetTeamName(NewName);
    }
    else
    {
        NewName = TeamsStructA.TeamName;
        SpaceShipsInGame.Add(SpaceShipState);
        SpaceShipsInGame[0]->SetTeamName(NewName);
    } 
}


void ASpaceShipGameStateBase::IncrementTeamKillCount(ASpaceShipPlayerState* SpaceShipState)
{
    if(GetLocalRole() == ROLE_Authority)
    {
        if(SpaceShipState->GetTeamName() == TeamsStructA.TeamName)
        {
            TeamsStructA.TeamKills += 1;
        }
        else
        {
            TeamsStructB.TeamKills += 1;
        }       
    }
}


void ASpaceShipGameStateBase::CountDown()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        if(TimeRemaining > 0)
        {
            TimeRemaining--;
        }
        else
        {
            bGameRunning = false;
        }
    }
}

FString ASpaceShipGameStateBase::DisplayWinner()
{
    FString Winner = TEXT("Draw!");
    if(GetLocalRole() == ROLE_Authority)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if(TeamsStructA.TeamKills > TeamsStructB.TeamKills)
        {
            Winner = TeamsStructA.TeamName;
        }
        else if(TeamsStructA.TeamKills < TeamsStructB.TeamKills)
        {
            Winner = TeamsStructB.TeamName;
        }
    }
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpaceShipGameStateBase::CallRestart, 5, false);
    return Winner;
}

void ASpaceShipGameStateBase::CallRestart_Implementation()
{
    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if(ASpaceShipGameModeBase* MyGameMode = Cast<ASpaceShipGameModeBase>(GameMode))
	{
		MyGameMode->Restart();
	}
}


