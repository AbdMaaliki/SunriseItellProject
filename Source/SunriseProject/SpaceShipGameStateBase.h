// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TeamStruct.h"
#include "SpaceShipGameStateBase.generated.h"


class ASpaceShipPlayerState;

UCLASS()
class SUNRISEPROJECT_API ASpaceShipGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
ASpaceShipGameStateBase();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	//Struct for team A
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTeamStruct TeamsStructA;

	//Returns team A kills
	UFUNCTION(BlueprintPure, Category="Kills")
	FORCEINLINE int32 GetTeamAKills() const { return TeamsStructA.TeamKills; }

	//Struct for team B
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTeamStruct TeamsStructB;

	//Returns team A kills
	UFUNCTION(BlueprintPure, Category="Kills")
	FORCEINLINE int32 GetTeamBKills() const { return TeamsStructB.TeamKills; }

	//Returns a team's name
	//Note: I should've done the same with kills :)
	UFUNCTION(BlueprintPure, Category="Kills")
	FORCEINLINE FString GetTeamName(FTeamStruct Team) const { return Team.TeamName; }


	//Time remaining until the game ends
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)//ReplicatedUsing=OnRep_TimeRemaining
	float TimeRemaining;

	UFUNCTION(BlueprintPure, Category="Kills")
	FORCEINLINE float GetTimeRemaining() const { return TimeRemaining; }

	//Array of all players in game
	UPROPERTY(VisibleInstanceOnly)
	TArray<ASpaceShipPlayerState*> SpaceShipsInGame;

	//bool to stop countdown
	UPROPERTY(Replicated)
	bool bGameRunning = true;

	//returns bGameRunning
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetGameRunning() const{return bGameRunning;};

	//Add space ship to a team when game starts
	UFUNCTION()
	void AddSpaceShip(ASpaceShipPlayerState* SpaceShip);

	//increment kill counts of a ship's team
	UFUNCTION(BlueprintCallable)
	void IncrementTeamKillCount(ASpaceShipPlayerState* SpaceShip);

	//function called every second to update time remaining
	UFUNCTION()
	void CountDown();

	//function to display winner widget on all clients
	UFUNCTION(BlueprintCallable)
	void DisplayWinner();

	UFUNCTION(BlueprintCallable)
	void SetWinner(FString WinnerName);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString GetWinner(){return Winner;};

	//Server function to restart the game
	UFUNCTION(Server, Reliable)
	void CallRestart();

protected:

	virtual void BeginPlay() override;

private:

	//String used to update win screen
	UPROPERTY(Replicated)
	FString Winner;

};
