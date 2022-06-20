// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceShipPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUIDelegate);

UCLASS()
class SUNRISEPROJECT_API ASpaceShipPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Blueprint assignable widget class
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PlayerWidgetClass;	

	//Widget pointer to add it to viewport
	UUserWidget* Widget;

	//Called on each client to add the widget
	UFUNCTION(Client, reliable)
	void AddHUD();

	//Implemented in blueprints to update the hud every n seconds
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD();

	UFUNCTION(BlueprintPure)
	float GetHealth() const;


	//Delegate and it's function to update the HUD
	UFUNCTION(BlueprintCallable)
	void CallUpdateUIDelegate();
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FUpdateUIDelegate OnUpdateUIDelegate;

	//Returns player start location
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetStartLocation() const{ return StartLocation;};

	//Sets start location
	UFUNCTION(BlueprintCallable)
	void SetStartLocation(FVector Location);

protected:
	virtual void BeginPlay() override;

	//Start location assigned on begin play 
	//to use when respawning
	UPROPERTY(Replicated)
	FVector StartLocation;
};
