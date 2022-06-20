// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceShipPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRespawnDelegate);

UCLASS()
class SUNRISEPROJECT_API ASpaceShipPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PlayerWidgetClass;	

	UUserWidget* Widget;

	UFUNCTION(Client, reliable)
	void AddHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD();

	UFUNCTION(BlueprintPure)
	float GetHealth() const;


	//Delegate and it's function to update the HUD
	UFUNCTION(BlueprintCallable)
	void CallUpdateUIDelegate();
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FUpdateUIDelegate OnUpdateUIDelegate;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetStartLocation() const{ return StartLocation;};

	UFUNCTION(BlueprintCallable)
	void SetStartLocation(FVector Location);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	FVector StartLocation;
};
