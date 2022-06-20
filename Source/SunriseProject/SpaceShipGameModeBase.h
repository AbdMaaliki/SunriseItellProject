// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceShipPlayerController.h"
#include "SpaceShipGameModeBase.generated.h"


class ASpaceShipPlayerController;

UCLASS()
class SUNRISEPROJECT_API ASpaceShipGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	//Calls server function to respawn a player
	//using their player controller
	UFUNCTION(BlueprintCallable)
	void CallRespawn(ASpaceShipPlayerController* PlayerController);

	UFUNCTION()
	void ServerRespawn(ASpaceShipPlayerController* PlayerController);

	//Blueprint assignable class to spawn
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpaceShipCharacter> ShipToSpawnClass;

	//Game length in seconds
	UPROPERTY(EditDefaultsOnly)
	float GameLength = 90.f;

	
};
