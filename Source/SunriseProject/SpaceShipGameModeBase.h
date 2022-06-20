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

	UFUNCTION(BlueprintCallable)
	void CallRespawn(ASpaceShipPlayerController* PlayerController);

	UFUNCTION()
	void ServerRespawn(ASpaceShipPlayerController* PlayerController);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpaceShipCharacter> ShipToSpawnClass;

	UPROPERTY(EditDefaultsOnly)
	float GameLength = 90.f;

	
};
