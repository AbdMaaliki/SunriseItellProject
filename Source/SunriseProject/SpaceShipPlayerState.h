// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpaceShipPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SUNRISEPROJECT_API ASpaceShipPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	int32 PlayerKills = 0;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	FString TeamName;


public:

	ASpaceShipPlayerState();
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetPlayerKills(){return PlayerKills;};

	UFUNCTION(BlueprintCallable)
	void IncrementPlayerKills(int32 KillsCount);

	//Team name getter & setter
	void SetTeamName(FString NewTeamName);
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE FString GetTeamName() const { return TeamName; }

	UFUNCTION()
	void ResetValues();


};
